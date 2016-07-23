/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Paweł Stawicki. All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef STATICEDITMENU_H_
#define STATICEDITMENU_H_

#include <stdint.h>
#include "Menu.h"
#include "cprintf.h"


#define CE_STEP_TYPE_SMART      0x7fff
#define CE_STEP_TYPE_KEY_SPEED  0x7ffd
#define CE_STEP_TYPE_SIGNED     0x7ffc
#define CE_STEP_TYPE_METHOD     0x7ffe


#define EDIT_MENU_ALWAYS        0x7fff
#define EDIT_MENU_MANDATORY     0x8000
#define EDIT_MENU_LAST          0x0000

//TODO: rename
#define STATIC_EDIT_METHOD(method)  {CE_STEP_TYPE_METHOD,  {.editMethod=method}}
#define EDIT_STRING_ARRAY(x)        {CP_TYPE_STRING_ARRAY,0, {&x}}
#define EDIT_UINT32_ARRAY(x)        {CP_TYPE_UINT32_ARRAY,0, {&x}}


namespace EditMenu {

    typedef void(*EditCallBack)(uint16_t * value);
    typedef void(*EditMethod)(int dir);

    //TODO: rename EditData
    struct EditData {
        int16_t step;
        union {
            struct {
                uint16_t minValue;
                uint16_t maxValue;
            };
            EditMethod editMethod;
        };
    };
    struct StaticEditData {
        const char * staticString;
        uint16_t enableCondition;
        cprintf::PrintData print;
        EditData edit;
    };

    void initialize(const struct StaticEditData * staticEditData, const EditCallBack callback = NULL);
    uint16_t * getEditAddress(uint8_t item);
    uint16_t getEnableCondition(uint8_t item);
    void setSelector(uint16_t selector);


    inline int8_t run(bool alwaysRefresh = false)   { return Menu::run(alwaysRefresh); }
    inline bool runEdit()                           { return Menu::runEdit(); }

}
/*
class StaticEditMenu : public EditMenu {
public:


	struct StaticEditData {
	    const char * staticString;
	    uint16_t enableCondition;
		cprintf::PrintData print;
		EditData edit;
	};

public:
	StaticEditMenu(const StaticEditData * staticEditData, const EditCallBack callback = NULL)
			: EditMenu(NULL), staticEditData(staticEditData), selector(Always), editCallback(callback) {};
    virtual void printItem(uint8_t item);
    virtual void editItem(uint8_t item, uint8_t key);

    int16_t * getEditAddress(uint8_t item);
    uint16_t getEnableCondition(uint8_t item);

    void setSelector(uint16_t selector);

private:
    uint8_t getSelectedIndexOrSize(uint8_t item);
    const StaticEditData * staticEditData;
    uint16_t selector;
    EditCallBack editCallback;
};
*/
#endif /* STATICEDITMENU_H_ */
