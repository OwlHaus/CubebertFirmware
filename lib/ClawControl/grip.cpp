#include "grip.h"

#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/pwm.h"

Grip::Grip(int pin) {
    m_pin = pin;

    m_state = Grip::State::Unknown;

    set_pwm_pin(m_pin, nullptr);
}

Grip::~Grip() {
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
    switch (m_state) {
        case Grip::State::Open:
            pwm_set_gpio_level(m_pin, 0);
            break;
        case Grip::State::Closed:
            pwm_set_gpio_level(m_pin, 1000);
            break;
        default:
            break;
    }
}

void Grip::waitForReady() {
    
}

// Private Functions

void Grip::set_pwm_pin(int pin, irq_handler_t callback) {
    m_pin = pin;

    // Tell the LED pin that the PWM is in charge of its value.
    gpio_set_function(m_pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(m_pin);

    // Once I figure out what this stuff is form, maybe I'll use it.
    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    // pwm_clear_irq(slice_num);
    // pwm_set_irq_enabled(slice_num, true);
    // irq_set_exclusive_handler(PWM_IRQ_WRAP, callback);
    // irq_set_enabled(PWM_IRQ_WRAP, true);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 5.0);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);
}
