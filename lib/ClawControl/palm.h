#ifndef PALM_H
#define PALM_H

#include "fingers.h"

class Palm {
public:
    Palm(int dataPin1, int dataPin2);
    ~Palm();

    void home();

private:
    int m_state;

    Fingers* m_setOne;
    Fingers* m_setTwo;
};

#endif // PALM_H
