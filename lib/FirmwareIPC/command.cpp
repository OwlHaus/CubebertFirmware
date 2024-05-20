#include "command.h"

Command::Command(std::string command, std::string param) {
    m_action = ActionMap[command];
    m_param = param;
}

Command::~Command() {

}

Action Command::action() {
    return m_action;
}

std::string Command::param() {
    return m_param;
}
