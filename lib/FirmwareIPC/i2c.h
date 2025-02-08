#ifndef I2C_H
#define I2C_H

#include <queue>
#include "command.h"

typedef std::queue<Command*> CommandBuffer;

class I2C {
public:
    I2C();
    ~I2C();

    Command* next();

private:
    CommandBuffer* m_commandBuffer;

    void processQueue();
};

#endif // I2C_H
