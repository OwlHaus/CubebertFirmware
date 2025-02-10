#ifndef UART_H
#define UART_H

#include <cstdint>

#include "command.h"
#include "fipc.h"

class UART : public FIPC {
public:
    UART(uint8_t rx, uint8_t tx);
    ~UART();

    Command* next();

    void puts(std::string str);   

private:
    CommandBuffer* m_commandBuffer;

    int m_rx;
    int m_tx;

    void processQueue();
};

#endif // UART_H
