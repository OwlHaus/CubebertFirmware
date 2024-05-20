#include "state.h"

#include "command.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

#include "claw.h"
#include "config.h"
#include "wrist.h"

StateMachine::StateMachine(State initialState) {
    m_state = initialState;
    m_fipc = new FIPC();

    m_robot.north = new Claw(
        NORTH_FINGER1,
        NORTH_FINGER2,
        NORTH_STEP,
        NORTH_DIR,
        NORTH_ENABLE,
        NORTH_HOMING
    );

    m_robot.east = new Claw(
        EAST_FINGER1,
        EAST_FINGER2,
        EAST_STEP,
        EAST_DIR,
        EAST_ENABLE,
        EAST_HOMING
    );

    m_robot.south = new Claw(
        SOUTH_FINGER1,
        SOUTH_FINGER2,
        SOUTH_STEP,
        SOUTH_DIR,
        SOUTH_ENABLE,
        SOUTH_HOMING
    );

    m_robot.west = new Claw(
        WEST_FINGER1,
        WEST_FINGER2,
        WEST_STEP,
        WEST_DIR,
        WEST_ENABLE,
        WEST_HOMING
    );
}

StateMachine::~StateMachine() {
    delete m_fipc;

    delete m_robot.north;
    delete m_robot.east;
    delete m_robot.south;
    delete m_robot.west;
}

void StateMachine::setState(State state) {
    m_state = state;
}

State StateMachine::state() {
    return m_state;
}

void StateMachine::Home() {
    m_robot.north->home();
    m_robot.east->home();
    m_robot.south->home();
    m_robot.west->home();
}

void StateMachine::Turn(std::string param) {
    char ch = param.front();
    param.erase(param.begin());

    Claw* claw;
    switch(ch) {
        case 'n':
        case 'N':
            claw = m_robot.north;
            break;
        case 'e':
        case 'E':
            claw = m_robot.east;
            break;
        case 's':
        case 'S':
            claw = m_robot.south;
            break;
        case 'w':
        case 'W':
            claw = m_robot.west;
            break;
    };

    claw->turn(std::stoi(param));
}

void StateMachine::Ready() {
    Command* cmd = m_fipc->next();

    switch(cmd->action()) {
    case Action::Home: Home();
        break;
    case Action::Turn: Turn(cmd->param());
        break;
    // case Action::Clawffset: setClawffset(cmd->param());
    //     break;
    case Action::Solve:
        break;
    case Action::None:
    default:
        printf("No Command On Deck\n");
        break;
    }

    delete cmd;
}
