#ifndef FIPC_H
#define FIPC_H

#include <queue>
#include "command.h"

typedef std::queue<Command*> CommandBuffer;

class FIPC {
public:
    virtual Command* next();

    virtual void puts(std::string str);
};

#endif // FIPC_H