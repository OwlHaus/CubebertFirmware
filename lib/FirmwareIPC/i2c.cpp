#include "i2c.h"

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

I2C::I2C(uint8_t sda, uint8_t scl) {
    m_commandBuffer = new CommandBuffer();

    i2c_init(i2c0, 100000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);

    gpio_pull_up(sda);
    gpio_pull_up(scl);
    
    i2c_set_slave_mode(i2c0, true, 0x42);
}

I2C::~I2C() {
    delete m_commandBuffer;

    i2c_deinit(i2c0);
    gpio_set_function(0, GPIO_FUNC_SIO);
    gpio_set_function(1, GPIO_FUNC_SIO);
}

Command* I2C::next() {
    processQueue();

    Command* cmd = m_commandBuffer->front();
    m_commandBuffer->pop();
    
    if(cmd == nullptr) {
        printf("next() No CMD\n");
        cmd = new Command("", "");
    }
    return cmd;
}

//*******************//
// PRIVATE FUNCTIONS //
//*******************//

void I2C::processQueue() {
    std::string action = "";
    std::string buffer = "";
    
    bool readingCmd = false;
    while (true) {
        // This won't work, if there's no byte then it will read incorrectly
        // Fix by blocking or checking bytes, hard to know exactly what the fix
        // Is until I get the RPI to send data
        char pChar = static_cast<char>(i2c_read_byte_raw(i2c0));
        
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
