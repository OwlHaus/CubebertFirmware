#include "uart.h"

#include "pico/stdio.h"
#include "pico/stdlib.h"

UART::UART(uint rx, uint tx) {
    m_commandBuffer = new CommandBuffer();

    m_rx = rx;
    m_tx = tx;

    gpio_set_function(m_rx, UART_FUNCSEL_NUM(uart0, m_rx));
    gpio_set_function(m_tx, UART_FUNCSEL_NUM(uart0, m_tx));

    uart_init(uart0, 115200);
}

UART::~UART() {
    delete m_commandBuffer;

    uart_deinit(uart0);
    gpio_set_function(m_rx, GPIO_FUNC_SIO);
    gpio_set_function(m_tx, GPIO_FUNC_SIO);
}

Command* UART::next() {
    processQueue();

    Command* cmd = m_commandBuffer->front();
    m_commandBuffer->pop();
    
    if(cmd == nullptr) {
        puts("next() No CMD\r\n");
        cmd = new Command("", "");
    }
    return cmd;
}

void UART::puts(std::string str) {
    uart_puts(uart0, str.c_str());
}

//*******************//
// PRIVATE FUNCTIONS //
//*******************//

void UART::processQueue() {
    std::string action = "";
    std::string buffer = "";
    
    puts("processQueue()\r\n");
    bool readingCmd = false;
    for (int ch; (ch = uart_getc(uart0)); ch != EOF) {
        char pChar = static_cast<char>(ch);
        
        if (pChar == '*') {
            break;
        }

        switch (pChar) {
        case '^': readingCmd = true;
            break;
        case ',': std::swap(action, buffer);
            break;
        case '&':
            m_commandBuffer->push(new Command(action, buffer));
            action.clear();
            buffer.clear();
            readingCmd = false;
            break;
        default:
            if (readingCmd) {
                buffer.push_back(pChar);
            }
        }
    }
}
