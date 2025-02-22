#ifndef CLAW_H
#define CLAW_H

#include "grip.h"
#include "wrist.h"

struct WristConfig {
    uint stepPin;
    uint directionPin;
    uint sleepPin;
    uint homingPin;
};


// Enum for grip state
enum GripState {
    VerticalClosed,
    HorizontalClosed,
    AllClosed,
    AllOpen,
};

class Claw {
public:
    Claw(
        uint gripAPin,
        uint gripBPin,
        uint stepPin,
        uint directionPin,
        uint sleepPin,
        uint homingPin
    );
    ~Claw();

    void initialize();

    void home();
    void turn(int32_t deg);
    void grip(GripState gripState);
    void executeVerticalGrip();
    void executeHorizontalGrip();
    void waitForGripReady();
    void executeTurn();
    void waitForTurnReady();

private:
    Grip* m_gripA;
    Grip* m_gripB;
    Wrist* m_wrist;

    GripState m_gripState;

    Grip* getVerticalGrip();
    Grip* getHorizontalGrip();
};

#endif // CLAW_H
