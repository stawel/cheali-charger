#ifndef SCREEN_H_
#define SCREEN_H_

#include <inttypes.h>
#include "Program.h"
#include "Blink.h"

class Screen : public Blink {
public:
    uint16_t charge_;
    uint32_t startTime_totalTime_;
    uint32_t totalBalanceTime_;
    uint32_t totalChargDischargeTime_;
    bool on_;

    uint16_t getTimeSec() const;
    void doSlowInterrupt();

    static AnalogInputs::ValueType getI();

    void powerOn();
    void powerOff();
    Screen() {};
    enum ScreenViewType { Normal, Debug=0x80};
    enum ScreenType {
        ScreenStartInfo,
        ScreenFirst, ScreenCIVlimits, ScreenTime, ScreenTemperature,
        ScreenBalancer0_2,      ScreenBalancer3_5,
        ScreenBalancer0_2Rth,   ScreenBalancer3_5Rth,
        ScreenDeltaFirst,
        ScreenR,
        ScreenVout,
        ScreenVinput,
        ScreenDeltaVout,
        ScreenDeltaTextern,


        //Debug screens
        ScreenDebugRthVth = Debug,
        ScreenDebugI,
        ScreenDebugDelta,
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
    void displayDeltaVout();
    void displayDeltaTextern();
    void displayDeltaFirst();

    void displayScreenProgramCompleted();
    void displayDebugRthVth();
    void displayDebugI();
    void displayDebugDelta();

    void displayMonitorError();
    void displayStartInfo();

    void printCharge();
    void printChar_Time();

    static void displayStrings(const char *s1, const char *s2);
    static void displayNotImplemented();
    static void displayScreenReversedPolarity();

    static void runNotImplemented();
};

extern Screen screen;

#endif /* SCREEN_H_ */
