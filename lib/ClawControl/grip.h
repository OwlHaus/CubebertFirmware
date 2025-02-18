#ifndef GRIP_H
#define GRIP_H

#include "hardware/irq.h"

class Grip {
public:
    Grip(int pin);
    ~Grip();

    enum State {
        Unknown,
        Open,
        Closed
    };

    void home();
    void open();
    void close();
    void execute();
    void waitForReady();

private:
    int m_pin;
    State m_state;

    void set_pwm_pin(int pin, irq_handler_t callback);
};

#endif // GRIP_H
