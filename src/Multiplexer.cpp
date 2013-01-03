#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Multiplexer.h"
#include "Hardware.h"
#include "AnalogInputs.h"


inline void setAddress(uint8_t address)
{
    digitalWrite(MUX_ADR0_PIN, address&1);
    digitalWrite(MUX_ADR1_PIN, address&2);
    digitalWrite(MUX_ADR2_PIN, address&4);
}

void Multiplexer::init()
{
    pinMode(MUX_ADR0_PIN, OUTPUT);
    pinMode(MUX_ADR1_PIN, OUTPUT);
    pinMode(MUX_ADR2_PIN, OUTPUT);

    pinMode(MUX0_Z_D_PIN, INPUT);
    pinMode(MUX1_Z_D_PIN, INPUT);
}

uint8_t Multiplexer::digitalRead(uint8_t address)
{
    setAddress(address);
    uint8_t retu  = ::digitalRead(address&MUXINPUT1 ? MUX1_Z_D_PIN : MUX0_Z_D_PIN);
    return retu;
}

uint16_t Multiplexer::analogRead(uint8_t address)
{
    setAddress(address);
    if(address&MUXINPUT1)
        return AnalogInputs::analogValue<MUX1_Z_A_PIN>();
    else
        return AnalogInputs::analogValue<MUX0_Z_A_PIN>();
}
