#include <avr/io.h> 
#include <avr/interrupt.h> 


unsigned char USART_Receive(); 
void USARTinit(); 
void UART_Transmit(char); 


void main (void) 

{ 
   char buffer; 

   USARTinit(); 


   while(1) 
   { 
      buffer = USART_Receive(); 
      UART_Transmit (buffer); 
   } 
} 

void USARTinit() 
{ 
    // Increase the oscillator to 2 Mhz for the 38400 baudrate:  
    CLKPR = (1<<CLKPCE);        // set Clock Prescaler Change Enable 
    // set prescaler = 2, Inter RC 8Mhz / 2 = 4Mhz 
    CLKPR = (1<<CLKPS0);    

    // Set the USART baudrate registers for 38400k 
    UBRRH = 0;//(unsigned char)(baudrate>>8); 
    UBRRL = 12;//(unsigned char)baudrate; 

    // Enable 2x speed change 
    UCSRA = (1<<U2X); 

    // Enable receiver and transmitter 
    UCSRB = (1<<RXEN)|(1<<TXEN)|(0<<RXCIE)|(0<<UDRIE); 

    // Set the USART to asynchronous at 8 bits no parity and 1 stop bit 
    UCSRC = (0<<UMSEL)|(0<<UPM0)|(0<<USBS)|(3<<UCSZ0)|(0<<UCPOL); 
        
    // Enable interrupts 
    sei(); 
    
    // Setup the interrupt mask and flags 
   PCMSK1 = (1<<PINB6) | (1<<PINB4);       // set pin-change interrupt mask 
     EIFR   = (1<<PCIF1);                    // clear external intterupt flag 1 
   EIMSK  = (1<<PCIE1);                // enable external interrupt 1    
  
} 

void UART_Transmit (char buffer) 
{ 
   /*Wait for empty transmit buffer*/ 
   while (!(UCSRA & (1 << UDRE))) 
   ; 

   /*Put data into buffer, sends data*/ 
   UDR = buffer; 
} 

