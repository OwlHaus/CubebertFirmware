#ifndef STATE_H
#define STATE_H

#include "fipc.h"

enum State {
    Shutdown,
    Ready
};

// Forward Class desclarations
class Claw;

// Actual class
class StateMachine {
public:
    StateMachine(State initialState);
    ~StateMachine();

    void setState(State state);
    State state();

    void Home();
    void Turn(std::string param);
    void Ready();
private:
    struct Robot {
        Claw *north;
        Claw *south;
        Claw *east;
        Claw *west;
    };
    
    Robot m_robot;

    State m_state;
    FIPC* m_fipc;
};

#endif // STATE_H
