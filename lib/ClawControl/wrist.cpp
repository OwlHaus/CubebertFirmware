#include "wrist.h"

#include "stepper.pio.h"

#include <stdio.h>
#include "pico/time.h"
#include "pico/stdlib.h"

Wrist::Wrist(
    uint stepPin,
    uint directionPin,
    uint sleepPin,
    uint homingPin) {
    // Save Pin Info
    m_stepPin = stepPin;
    m_directionPin = directionPin;
    m_sleepPin = sleepPin;
    m_homingPin = homingPin;
    m_angle = 0;
    m_speed = 0;
    m_enabled = false;

    // Initialize Pins
    m_pio = pio0;
    m_stateMachine = pio_claim_unused_sm(m_pio, true);
    m_programOffset = pio_add_program(m_pio, &stepper_program);
    stepper_program_init(m_pio, m_stateMachine, m_programOffset, m_stepPin, m_homingPin);
    pio_sm_set_enabled(m_pio, m_stateMachine, true);

    gpio_init(m_directionPin);
    gpio_set_dir(m_directionPin, GPIO_OUT);

    gpio_init(m_sleepPin);
    gpio_set_dir(m_sleepPin, GPIO_OUT);
    gpio_put(m_sleepPin, 0); // Start with the motor asleep
}

Wrist::~Wrist() {
    
}

void Wrist::home() {
    // Begin Homing
    gpio_put(m_directionPin, Direction::CW);

    enable();
    // Start Homing Sequence
    pio_sm_put(m_pio, m_stateMachine, 0);
    // Set steps and speed, we should find the switch within 360 degrees
    pio_sm_put(m_pio, m_stateMachine, angle2Steps(360));
    pio_sm_put(m_pio, m_stateMachine, HOMING_SPEED);
    
    // setSpeed(HOMING_SPEED / 2);
    // gpio_put(m_directionPin, Direction::CW);

    pio_sm_get_blocking(m_pio, m_stateMachine);
    disable();

    m_angle = 0;
}

uint32_t Wrist::getAngle() {
    return m_angle;
}

/**
 * @brief Sets the Angle and 
 * 
 * @param angle - in degrees
 */
void Wrist::turn(int32_t angle) {
    gpio_put(m_directionPin, (angle > 0 ? Direction::CW : Direction::CCW));
    m_angle = angle;
}

void Wrist::execute() {
    if (m_angle == 0) {
        return;
    }

    enable();
    pio_sm_put(m_pio, m_stateMachine, angle2Steps(m_angle));
    pio_sm_put(m_pio, m_stateMachine, m_speed);
}

/**
 * @brief Sets the speed where 1 is max speed
 *
 * For the DRV8834 The min required pulse width is 1.9us high and low
 * we're going to hedge our bets and do 2us giving us a MAX freq of 250kHz
 * there's no way the motor can go that fast, and calculations lead me to believe
 * for a first go between 2.5kHz and 20kHz is ideal ranging from 0.66 tps to 6 tps
 * tps - turn per second where 1 turns is 360 degrees
 * overall this gives us a no-op length between 208 and 52 where longer lengths are slower frequencies
 * 
 * @param speed 
 */
void Wrist::setSpeed(float speed) {
    if (speed > 0 && speed <= 1) {
        m_speed = (NOOP_RANGE * speed) + 52;
    }
}

/**
 * @brief waits until the motor is ready to accept commands
 */
void Wrist::waitForReady() {
    if (m_enabled) {
        pio_sm_get_blocking(m_pio, m_stateMachine);
        disable();
    }
}

// ------ Private ------ //

uint32_t Wrist::pioBlockCounter = 0;

/**
 * @brief brings the sleep pin high, costing some power, but at least we can turn
 * 
 */
void Wrist::enable() {
    m_enabled = true;
    gpio_put(m_sleepPin, 1);
    sleep_ms(25); // Wait for DRV8835 to wake up
}

/**
 * @brief brings the sleep pin low, saving some power at the cost of torque
 * 
 */
void Wrist::disable() {
    m_enabled = false;
    gpio_put(m_sleepPin, 0);
}

/**
 * @brief Converts the given angle in Degrees to Steps based on Resolution
 * 
 * @param angle - angle in degrees
 * @return uint32_t - the number of steps that correlates to
 */
uint32_t Wrist::angle2Steps(uint32_t angle) {
    return static_cast<uint32_t>(angle / (DEG_PER_STEP * STEP_RESOLUTION));
}