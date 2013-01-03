#ifndef PROGRAMDATAMENU_H_
#define PROGRAMDATAMENU_H_

#include "ProgramData.h"
#include "EditMenu.h"

#define PROGRAM_DATA_MENU_SIZE 8

class ProgramDataMenu : public EditMenu {
public:
    ProgramDataMenu(const ProgramData &p, int programIndex);
    virtual uint8_t printItem(uint8_t i);
    virtual void editItem(uint8_t i, uint8_t key);

    bool run();

    void editName();
    void createName();

    ProgramData p_;
    int programIndex_;
};

#endif /* PROGRAMDATAMENU_H_ */
