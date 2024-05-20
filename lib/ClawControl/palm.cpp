#include "palm.h"

Palm::Palm(int dataPin1, int dataPin2) {
    m_setOne = new Fingers(dataPin1);
    m_setTwo = new Fingers(dataPin2);
}

Palm::~Palm() {
    delete m_setOne;
    delete m_setTwo;
}

void Palm::home() {
    m_setOne->open();
    m_setTwo->open();
}
