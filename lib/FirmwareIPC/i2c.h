#ifndef I2C_H
#define I2C_H

#include <cstdint>

#include "command.h"
#include "fipc.h"

class I2C : public FIPC {
public:
    I2C(uint8_t sda, uint8_t scl);
    ~I2C();

    Command* next();

private:
    CommandBuffer* m_commandBuffer;

    void processQueue();
};

#endif // I2C_H
