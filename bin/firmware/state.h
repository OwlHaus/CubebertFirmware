#ifndef STATE_H
#define STATE_H

#include "fipc.h"
#include "grip.h"
#include "rcsp.h"

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
    void Clawffset(std::string param);
    void Solve(std::string param);
    void Lift(std::string param);
    void Ready();

private:
    struct Robot {
        Claw *north;
        Claw *south;
        Claw *east;
        Claw *west;
        Grip *lift;
    };
    
    Robot m_robot;

    State m_state;
    FIPC* m_fipc;
    RCSP* m_rcsp;
};

#endif // STATE_H
