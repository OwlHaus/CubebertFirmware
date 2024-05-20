#include "fingers.h"

#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/pwm.h"

Fingers::Fingers(int pin) {
    m_pin = pin;

    m_state = Fingers::State::Unknown;
}

Fingers::~Fingers() {
}

void Fingers::open() {
    m_state = Fingers::State::Open;
}

void Fingers::close() {
    m_state = Fingers::State::Closed;
}

// Private Functions

void Fingers::set_pwm_pin(int pin, irq_handler_t callback) {
    // Tell the LED pin that the PWM is in charge of its value.
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, callback);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 5.0);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);
}
