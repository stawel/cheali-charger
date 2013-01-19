#include "AnalogInputs.h"
#include "Hardware.h"

#include "Monitor.h"
#include "Utils.h"
#include "Settings.h"
#include "ProgramData.h"

Monitor monitor;


void Monitor::doInterrupt()
{
    bool on;
    if(testTintern(on, settings.fanTempOn_ - Settings::TempDifference, settings.fanTempOn_))
        hardware::setFan(on);
}

void Monitor::powerOn() {
    VoutMaxMesured_ = analogInputs.reverseCalibrateValue(AnalogInputs::Vout, MAX_CHARGE_V);
}


Strategy::statusType Monitor::run()
{
    AnalogInputs::ValueType t = analogInputs.getRealValue(AnalogInputs::Tintern);

    if(t > settings.dischargeTempOff_+Settings::TempDifference) {
        Program::stopReason_ = PSTR("intern T");
        return Strategy::ERROR;
    }

    AnalogInputs::ValueType VMout = analogInputs.getMeasuredValue(AnalogInputs::Vout);
    if(VMout > VoutMaxMesured_) {
        Program::stopReason_ = PSTR("bat disc");
        return Strategy::ERROR;
    }

    AnalogInputs::ValueType Vin = analogInputs.getRealValue(AnalogInputs::Vin);
    if(analogInputs.isConnected(AnalogInputs::Vin) && Vin < settings.inputVoltageLow_) {
        Program::stopReason_ = PSTR("input V");
        return Strategy::ERROR;
    }

    uint16_t c=0;
    getCharge(c);
    if(c> ProgramData::currentProgramData.getCapacityLimit()) {
        Program::stopReason_ = PSTR("cap COFF");
        return Strategy::COMPLETE;
    }
    if(settings.externT_) {
        AnalogInputs::ValueType Textern = analogInputs.getRealValue(AnalogInputs::Textern);
        if(Textern > settings.externTCO_) {
            Program::stopReason_ = PSTR("ext TCOF");
            return Strategy::COMPLETE;
        }
    }

    return Strategy::RUNNING;
}

