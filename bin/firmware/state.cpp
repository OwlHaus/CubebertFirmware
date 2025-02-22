#include "state.h"

#include <format>

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

    m_robot.lift = new Grip(LIFT_PIN);

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
        case 'N': claw = m_robot.north; break;
        case 'E': claw = m_robot.east; break;
        case 'S': claw = m_robot.south; break;
        case 'W': claw = m_robot.west; break;
    };

    claw->turn(std::stoi(param));
}

void StateMachine::Clawffset(std::string param) {
    m_fipc->puts("Clawffset Not Implemented Yet\r\n");
}

void StateMachine::Solve(std::string param) {
    m_fipc->puts("Solving\r\n");
    
    std::queue<std::string> moves;
    while(param.length() != 0) {
        moves.push(param.substr(0, param.find(' ')));
        if (param.find(' ') == std::string::npos) {
            break;
        }
        param = param.substr(param.find(' ') + 1);
    }

    std::queue<Move*> *moveQueue = m_rcsp->parseSolve(moves);

    Move *move = moveQueue->front();
    moveQueue->pop();
    while (move != nullptr) {
        m_fipc->puts(m_rcsp->moveToString(move));

        m_robot.east->grip(move->eastGrip);
        m_robot.west->grip(move->westGrip);
        m_robot.north->grip(move->northGrip);
        m_robot.south->grip(move->southGrip);
        m_fipc->puts("Gripping\r\n");

        m_robot.north->executeVerticalGrip();
        m_robot.south->executeVerticalGrip();
        m_robot.east->executeVerticalGrip();
        m_robot.west->executeVerticalGrip();
        m_fipc->puts("Vertical Grip\r\n");


        m_robot.north->waitForGripReady();
        m_robot.south->waitForGripReady();
        m_robot.east->waitForGripReady();
        m_robot.west->waitForGripReady();
        m_fipc->puts("Vertical Grip Ready\r\n");

        m_robot.north->executeHorizontalGrip();
        m_robot.south->executeHorizontalGrip();
        m_robot.east->executeHorizontalGrip();
        m_robot.west->executeHorizontalGrip();
        m_fipc->puts("Horizontal Grip\r\n");
        
        m_robot.north->waitForGripReady();
        m_robot.south->waitForGripReady();
        m_robot.east->waitForGripReady();
        m_robot.west->waitForGripReady();
        m_fipc->puts("Horizontal Grip Ready\r\n");

        m_robot.east->turn(move->eastGripRotation);
        m_robot.west->turn(move->westGripRotation);
        m_robot.north->turn(move->northGripRotation);
        m_robot.south->turn(move->southGripRotation);
        m_fipc->puts("Turning\r\n");

        m_robot.east->executeTurn();
        m_robot.west->executeTurn();
        m_robot.north->executeTurn();
        m_robot.south->executeTurn();
        m_fipc->puts("Turned\r\n");

        m_robot.east->waitForTurnReady();
        m_fipc->puts("East Turned\r\n");
        m_robot.west->waitForTurnReady();
        m_fipc->puts("West Turned\r\n");
        m_robot.north->waitForTurnReady();
        m_fipc->puts("North Turned\r\n");
        m_robot.south->waitForTurnReady();
        m_fipc->puts("South Turned\r\n");

        if(moveQueue->empty()) {
            break;
        }

        delete move;
        move = moveQueue->front();
        moveQueue->pop();
    }

    m_fipc->puts("Done Solving\n");
}

void StateMachine::Lift(std::string param) {
    m_fipc->puts("StateMachine::Lift()\r\n");
    switch(param.front()) {
        case 'U': m_robot.lift->open(); break;
        case 'D': m_robot.lift->close(); break;
        default:
            m_fipc->puts("Lift Not Implemented Yet\r\n");
            break;
    };
    m_robot.lift->execute();
    m_robot.lift->waitForReady();
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
    case Action::Lift: Lift(cmd->param());
        break;
    case Action::None:
    default:
        m_fipc->puts("No Command On Deck\n");
        break;
    }

    delete cmd;
}