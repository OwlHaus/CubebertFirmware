#include "claw.h"

Claw::Claw(
    uint fingerPin1,
    uint fingerPin2,
    uint stepPin,
    uint directionPin,
    uint sleepPin,
    uint homingPin) {
}

Claw::~Claw() {
    delete m_palm;
    delete m_wrist;
}

void Claw::initialize() {
    home();
}

void Claw::home() {
    m_palm->home();
    m_wrist->home();
}

void Claw::turn(int32_t deg) {
}