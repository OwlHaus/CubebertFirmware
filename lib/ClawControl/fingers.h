#ifndef FINGERS_H
#define FINGERS_H

#include "hardware/irq.h"

class Fingers {
public:
    Fingers(int pin);
    ~Fingers();

    enum State {
        Unknown,
        Open,
        Closed
    };

    void open();
    void close();

private:
    int m_pin;
    State m_state;

    void set_pwm_pin(int pin, irq_handler_t callback);
};

#endif // FINGERS_H
