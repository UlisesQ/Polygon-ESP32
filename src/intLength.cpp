#include "intLength.h"
#include <cstdlib> //for abs


int intLength(int myInt) {
    myInt = abs(myInt);
    if (myInt >= 10000) {
        return 5;
    } else if (myInt >= 1000) {
        return 4;
    } else if (myInt >= 100) {
        return 3;
    } else if (myInt >= 10) {
        return 2;
    } else {
        return 1;
    }
}