#include "grip.h"

#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/pwm.h"

Grip::Grip(int pin) {
    m_pin = pin;

    m_state = Grip::State::Unknown;

    m_servo = new Servo();
    m_servo->attach(m_pin);
}

Grip::~Grip() {
    delete m_servo;
}

void Grip::home() {
    m_state = Grip::State::Open;
    execute();
}

void Grip::open() {
    m_state = Grip::State::Open;
}

void Grip::close() {
    m_state = Grip::State::Closed;
}

void Grip::execute() {
    m_servo->write(m_state);
}

void Grip::waitForReady() {
    
}
