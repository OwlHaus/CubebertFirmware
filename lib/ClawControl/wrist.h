#ifndef WRIST_H
#define WRIST_H

#include "hardware/pio.h"

#define HOMING_SPEED   720
#define HOMING_POLL_MS 100u

#define HALF_STEP            0.5f
#define QUARTER_STEP         0.25f
#define EIGTH_STEP           0.125f
#define THIRTY_SECONDTH_STEP 0.03125f

#define NOOP_MAX   208u
#define NOOP_MIN   52u
#define NOOP_RANGE (NOOP_MAX - NOOP_MIN)

#define DEG_PER_STEP         1.8f
#define STEP_RESOLUTION      QUARTER_STEP
#define STEPS_PER_REVOLUTION (360 / (DEG_PER_STEP * STEP_RESOLUTION))

// no clue if this is correct, I have no clue what the state machine numbers actually are
#define pis_smX_rx_fifo_not_empty(sm) ( \
    (sm) == 0 ? pio_interrupt_source::pis_sm0_rx_fifo_not_empty : \
    (sm) == 1 ? pio_interrupt_source::pis_sm1_rx_fifo_not_empty : \
    (sm) == 2 ? pio_interrupt_source::pis_sm2_rx_fifo_not_empty : \
    (sm) == 3 ? pio_interrupt_source::pis_sm3_rx_fifo_not_empty : \
    -1) \
)

class Wrist {
public:
    Wrist(
        uint stepPin,
        uint directionPin,
        uint sleepPin,
        uint homingPin);
    ~Wrist();

    void home();

    void turn(int32_t deg);
    void execute();
    void setSpeed(float speed); // (0.0, 1.0]

    void waitForReady();

private:
    static uint32_t pioBlockCounter;

    uint m_stepPin;
    uint m_directionPin;
    uint m_sleepPin;
    uint m_homingPin;

    uint32_t m_speed;
    uint32_t m_angle;
    bool m_enabled;

    PIO m_pio;
    uint32_t m_stateMachine;
    uint32_t m_programOffset;

    void enable();
    void disable();
    uint32_t angle2Steps(uint32_t angle);

    enum Direction {
        CW,
        CCW
    };
};

#endif // WRIST_H
