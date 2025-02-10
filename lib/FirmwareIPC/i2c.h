#ifndef I2C_H
#define I2C_H

#include "command.h"
#include "fipc.h"
#include "pico/stdio.h"

class I2C : public FIPC {
public:
    I2C(uint sda, uint scl);
    ~I2C();

    Command* next();

private:
    CommandBuffer* m_commandBuffer;

    void processQueue();
};

#endif // I2C_H
