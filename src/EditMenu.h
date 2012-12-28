#ifndef EDITMENU_H_
#define EDITMENU_H_

#include "StaticMenu.h"
#include "Blink.h"


class EditMenu : public StaticMenu, public Blink {
public:
	EditMenu(const char * const* staticMenu,uint8_t size): StaticMenu(staticMenu, size){}
	virtual uint8_t printItem(uint8_t item) {}
	virtual void editItem(uint8_t item, uint8_t key) {}
	bool runEdit(uint8_t index);
};

#endif /* EDITMENU_H_ */
