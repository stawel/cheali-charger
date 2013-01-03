#ifndef THEVENINMETHOD_H_
#define THEVENINMETHOD_H_

#include "Hardware.h"
#include "Thevenin.h"

class TheveninMethod {
public:
    static const AnalogInputs::Name VName = AnalogInputs::VoutBalancer;
    enum FallingState {NotFalling, LastRthMesurment, Falling};
    uint16_t minValue_;
    uint16_t maxValue_;
    AnalogInputs::ValueType Vend_;

    Thevenin tVout_;
    Thevenin tBal_[MAX_BANANCE_CELLS];
    FallingState Ifalling_;
    uint8_t fullCount_;
    uint8_t cells_;

    TheveninMethod(){}

    void init();
    bool isComlete(bool isEndVout, AnalogInputs::ValueType value);
    void calculateRthVth(AnalogInputs::ValueType oldValue);
    AnalogInputs::ValueType calculateNewValue(bool isEndVout, AnalogInputs::ValueType oldValue);
    AnalogInputs::ValueType calculateI();
    AnalogInputs::ValueType normalizeI(AnalogInputs::ValueType value, AnalogInputs::ValueType oldValue);
    AnalogInputs::ValueType storeOldValue(AnalogInputs::ValueType oldValue);

    static AnalogInputs::ValueType getVout() { return analogInputs.getRealValue(VName); }

    void setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i);
    void setMinI(AnalogInputs::ValueType i) {    minValue_ = i; };

};

extern TheveninMethod theveninMethod;


#endif /* THEVENINCHARGE_H_ */
