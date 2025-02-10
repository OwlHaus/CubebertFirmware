#ifndef UART_H
#define UART_H

#include "command.h"
#include "fipc.h"
#include "pico/stdio.h"

class UART : public FIPC {
public:
    UART(uint rx, uint tx);
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
