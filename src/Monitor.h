#ifndef MONITOR_H_
#define MONITOR_H_


class Monitor {
public:
	enum statusType {OK, ERROR};
	Monitor(){};
	statusType run();
	void doInterrupt();

protected:
	statusType monitorInternalTemperature();
//	statusType monitorInputVoltage();
//	statusType monitorOutputPolarity();

};

extern Monitor monitor;

#endif /* MONITOR_H_ */

