#ifndef MONITOR_H_
#define MONITOR_H_


class Monitor {
public:
    uint16_t VoutMaxMesured_;
    Monitor(){};
    Strategy::statusType run();
    void doInterrupt();
    void powerOn();
};

extern Monitor monitor;

#endif /* MONITOR_H_ */

