#ifndef UART_H
#define UART_H

#include <queue>
#include "command.h"

typedef std::queue<Command*> CommandBuffer;

class UART {
public:
    UART();
    ~UART();

    Command* next();

private:
    CommandBuffer* m_commandBuffer;

    void processQueue();
};

#endif // UART_H
