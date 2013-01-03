#ifndef EDITNAME_H_
#define EDITNAME_H_

#include <inttypes.h>

#define MAX_EDIT_NAME 16

class EditName {
    char * name_;
    uint8_t size_;
    const char * printStringP_;
    uint8_t cursor_;
    uint8_t beginSL_;
    uint8_t cursorSL_;
public:
    EditName(char * name, int size, const char *printStringP);

    bool run();
    void mainScreen();
    void selectLetter();
    void selectLetterScreen();
    void save();
    void incSL();
    void decSL();
    uint8_t getSL();
};

#endif /* EDITNAME_H_ */
