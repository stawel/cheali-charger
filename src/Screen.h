#ifndef SCREEN_H_
#define SCREEN_H_

#include <inttypes.h>
#include "Program.h"
#include "Blink.h"

class Screen : public Blink {
public:
    int16_t Rth_V_;
    int16_t Rth_I_;
    AnalogInputs::ValueType Vth_;
    AnalogInputs::ValueType valueTh_;
    uint16_t charge_;
    Program::ProgramType programType_;
    uint32_t startTime_totalTime_;
    uint32_t totalBalanceTime_;
    uint32_t totalChargDischargeTime_;
    AnalogInputs::Name iName_;
    bool on_;

    uint16_t getTimeSec() const;
    void doSlowInterrupt();

    void powerOn();
    void powerOff();
    Screen() {};
    enum ScreenViewType { Normal, Debug=0x80};
    enum ScreenType {
        ScreenStartInfo,
        ScreenFirst, ScreenCIVlimits, ScreenTime, ScreenTemperature,
        ScreenBalancer0_2,      ScreenBalancer3_5,
        ScreenBalancer0_2Rth,   ScreenBalancer3_5Rth,
        ScreenR,
        ScreenVout,
        ScreenVinput,

        //Debug screens
        ScreenDebugRthVth = Debug,
        ScreenDebugI,
        ScreenDebugBalancer0_2M,    ScreenDebugBalancer3_5M,
        ScreenDebugBalancer0_2RthV, ScreenDebugBalancer3_5RthV,
        ScreenDebugBalancer0_2RthI, ScreenDebugBalancer3_5RthI,
        };

    void display(ScreenType screen);

    void displayScreenFirst();
    void displayScreenCIVlimits();
    void displayScreenTime();
    void displayScreenTemperature();
    void displayScreenR();
    void displayScreenVout();
    void displayScreenVinput();

    void displayScreenProgramCompleted();
    void displayDebugRthVth();
    void displayDebugI();

    void displayMonitorError();
    void displayStartInfo();

    static void displayStrings(const char *s1, const char *s2);
    static void displayNotImplemented();
    static void displayScreenReversedPolarity();

    static void runNotImplemented();
};

extern Screen screen;

#endif /* SCREEN_H_ */
