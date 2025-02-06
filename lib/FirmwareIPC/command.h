#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <map>

enum Action {
    None,
    Home,
    Clawffset,
    Turn,
    Solve,
};

class Command {
public:
    Command(std::string command, std::string param);
    ~Command();

    Action action();
    std::string param();

private:
    std::map<std::string, Action> ActionMap{
        { "",          Action::None},
        { "Home",      Action::Home },
        { "Clawffset", Action::Clawffset },
        { "Turn",      Action::Turn },
        { "Solve",     Action::Solve },
    };

    Action m_action;
    std::string m_param;
};

#endif // COMMAND_H
