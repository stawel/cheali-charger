
//#define NULL 0
#include <stddef.h>

namespace ala {
    void f();
typedef void (*fun)();
    struct Menu {
        fun g;
    };
}


struct  ala::Menu table[] = { 
    {ala::f},
    { NULL}
};

