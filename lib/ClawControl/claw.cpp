#include "claw.h"

Claw::Claw(
    uint gripAPin,
    uint gripBPin,
    uint stepPin,
    uint directionPin,
    uint sleepPin,
    uint homingPin) {
    m_gripA = new Grip(gripAPin);
    m_gripB = new Grip(gripBPin);
    m_wrist = new Wrist(stepPin, directionPin, sleepPin, homingPin);
    m_wrist->setSpeed(0.5);
    m_gripState = GripState::AllOpen;
}

Claw::~Claw() {
    delete m_gripA;
    delete m_gripB;
    delete m_wrist;
}

void Claw::initialize() {
    home();
}

void Claw::home() {
    m_gripA->home();
    m_gripB->home();
    m_wrist->home();
}

void Claw::turn(int32_t deg) {
    m_wrist->turn(deg);
}

void Claw::grip(GripState gripState) {
    m_gripState = gripState;
    switch(gripState) {
        case VerticalClosed:
            getVerticalGrip()->close();
            getHorizontalGrip()->open();
            break;
        case HorizontalClosed:
            getVerticalGrip()->open();
            getHorizontalGrip()->close();
            break;
        case AllClosed:
            getVerticalGrip()->close();
            getHorizontalGrip()->close();
            break;
        case AllOpen:
            getVerticalGrip()->open();
            getHorizontalGrip()->open();
            break;
    }
}

void Claw::executeVerticalGrip() {
    getVerticalGrip()->execute();
}

void Claw::executeHorizontalGrip() {
    getHorizontalGrip()->execute();
}

void Claw::waitForGripReady() {
    getVerticalGrip()->waitForReady();
    getHorizontalGrip()->waitForReady();
}

void Claw::executeTurn() {
    m_wrist->execute();
}

void Claw::waitForTurnReady() {
    m_wrist->waitForReady();
}

Grip* Claw::getVerticalGrip() {
    // if angle is between 315 and 45 or between 135 and 225 gripA is vertical
    // else gripB is vertical
    if ((m_wrist->getAngle() >= 315 && m_wrist->getAngle() <= 360)
            || (m_wrist->getAngle() >= 0 && m_wrist->getAngle() <= 45)
            || (m_wrist->getAngle() >= 135 && m_wrist->getAngle() <= 225)) {
        return m_gripA;
    } else {
        return m_gripB;
    }
}

Grip* Claw::getHorizontalGrip() {
    // if angle is between 45 and 135 or between 225 and 315 gripA is horizontal
    // else gripB is horizontal
    if ((m_wrist->getAngle() >= 45 && m_wrist->getAngle() <= 135)
            || (m_wrist->getAngle() >= 225 && m_wrist->getAngle() <= 315)) {
        return m_gripA;
    } else {
        return m_gripB;
    }
}
