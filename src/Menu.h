#ifndef MENU_H_
#define MENU_H_

#include <inttypes.h>

class Menu {
public:
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
	virtual void printItem(int i) {}
	uint8_t getMenuSize() const { return size_; }

	void display();
};

#endif /* MENU_H_ */

