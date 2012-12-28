#ifndef PROGRAMDATAMENU_H_
#define PROGRAMDATAMENU_H_

#include "StaticMenu.h"
#include "ProgramData.h"
#include "Blink.h"

#define PROGRAM_DATA_MENU_SIZE 8

class ProgramDataMenu : public StaticMenu, public Blink {
public:
	ProgramDataMenu(const ProgramData &p, int programIndex);
	virtual uint8_t printItem(uint8_t i);
	void editIndex(uint8_t i);
	bool editItem(uint8_t i, uint8_t key);

	bool edit();
	void editName();
	void createName();

	ProgramData p_;
	int programIndex_;
};

#endif /* PROGRAMDATAMENU_H_ */
