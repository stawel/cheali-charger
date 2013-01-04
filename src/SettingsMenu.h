#ifndef SETTINGSMENU_H_
#define SETTINGSMENU_H_

#include "Settings.h"
#include "EditMenu.h"

class SettingsMenu : public EditMenu {
public:
    SettingsMenu(const Settings &p);
    virtual uint8_t printItem(uint8_t i);
    virtual void editItem(uint8_t i, uint8_t key);

    bool run();

    Settings p_;
private:

    void printViewType() const;
    void changeBacklight(int dir);
    void changeViewType(int dir);

    static void printTemp(AnalogInputs::ValueType t);
    static void printVolt(AnalogInputs::ValueType v);
    static void changeTemp(AnalogInputs::ValueType &v, int dir);
    static void changeVolt(AnalogInputs::ValueType &v, int dir);
};

#endif /* SETTINGSMENU_H_ */
