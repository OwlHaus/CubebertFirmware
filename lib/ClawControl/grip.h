#ifndef GRIP_H
#define GRIP_H

#include "hardware/irq.h"
#include "Servo.h"

class Grip {
public:
    Grip(int pin);
    ~Grip();

    enum State {
        Unknown = 0,
        Open = 180,
        Closed = 0,
    };

    void home();
    void open();
    void close();
    void execute();
    void waitForReady();

private:
    int m_pin;
    State m_state;
    Servo *m_servo;
};

#endif // GRIP_H
