#ifndef MENU_H_
#define MENU_H_

#include <inttypes.h>
#include "PolarityCheck.h"

class Menu : public PolarityCheck {
public:
	static const int MENU_EXIT = -1;

	uint8_t pos_;
	uint8_t begin_;
	uint8_t size_;
	bool render_;
	bool waitRelease_;
public:
	Menu(uint8_t size);
	void render() { render_ = true; }

	uint8_t run();
	int runSimple();

	void incIndex();
	void decIndex();

	uint8_t getIndex() { return begin_ + pos_; }
	virtual uint8_t printItem(uint8_t i) {}
	uint8_t getMenuSize() const { return size_; }

	void display();
};

#endif /* MENU_H_ */

