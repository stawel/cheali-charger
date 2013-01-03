#ifndef MULTIPLEXER_H_
#define MULTIPLEXER_H_

class Multiplexer {
public:
    static void init();
    static uint8_t digitalRead(uint8_t address);
    static uint16_t analogRead(uint8_t address);


    template<uint8_t address>
    static uint16_t analogRead() {
        return analogRead(address);
    }
    template<uint8_t address>
    static uint8_t digitalRead() {
        return digitalRead(address);
    }
};



#endif /* MULTIPLEXER_H_ */
