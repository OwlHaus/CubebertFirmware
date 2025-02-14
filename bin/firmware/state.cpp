#include "state.h"

#include "command.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

#include "claw.h"
#include "config.h"
#include "i2c.h"
#include "uart.h"
#include "wrist.h"

StateMachine::StateMachine(State initialState) {
    m_state = initialState;
    
#ifdef CUBEBERT_FIPC_UART
    m_fipc = new UART(0, 1);
#else
    m_fipc = new I2C(2, 3);
#endif
    m_fipc->puts("Initialize State Machine\n");

    m_rcsp = new RCSP(m_fipc);

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

    //claw->turn(std::stoi(param));
}

void StateMachine::Clawffset(std::string param) {
    m_fipc->puts("Clawffset\n");
}

void StateMachine::Solve(std::string param) {
    m_fipc->puts("Solving\n");
    
    std::queue<std::string> moves;
    while(param.length() != 0) {
        moves.push(param.substr(0, param.find(' ')));
        if (param.find(' ') == std::string::npos) {
            break;
        }
        param = param.substr(param.find(' ') + 1);
    }

    std::queue<Move*> *moveQueue = m_rcsp->parseSolve(moves);
    m_rcsp->printMoveQueue(moveQueue);

    m_fipc->puts("Done Solving\n");
}

void StateMachine::Ready() {
    m_fipc->puts("Ready\n");
    Command* cmd = m_fipc->next();

    switch(cmd->action()) {
    case Action::Home: Home();
        break;
    case Action::Turn: Turn(cmd->param());
        break;
    case Action::Clawffset: Clawffset(cmd->param());
        break;
    case Action::Solve: Solve(cmd->param());
        break;
    case Action::None:
    default:
        m_fipc->puts("No Command On Deck\n");
        break;
    }

    delete cmd;
}
