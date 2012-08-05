#ifndef PROGRAMDATAMENU_H_
#define PROGRAMDATAMENU_H_

#include "Menu.h"
#include "ProgramData.h"

class ProgramDataMenu : public Menu {
public:
	ProgramDataMenu(const ProgramData &p): Menu(9), p_(p), blinkIndex_(-1) {};
	virtual void printItem(int i);
	void editIndex(int i);
	bool editItem(int i, uint8_t key);

	bool edit();
	bool getBlink() const;
	int getBlinkIndex();
	void startBlink(int index, bool off);
	void stopBlink();
	void blinkDisplay();

	ProgramData p_;
	int blinkIndex_;
	bool blinkOff_;
	bool blinkStartOff_;
	uint16_t blinkStartTime_;
};

#endif /* PROGRAMDATAMENU_H_ */
