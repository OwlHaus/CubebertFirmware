#include "fipc.h"

#include "pico/stdio.h"

FIPC::FIPC() {
    m_commandBuffer = new CommandBuffer();
}

FIPC::~FIPC() {
    delete m_commandBuffer;
}

Command* FIPC::next() {
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

void FIPC::processQueue() {
    std::string action = "";
    std::string buffer = "";
    
    bool readingCmd = false;
    for (int ch; (ch = getchar()); ch != EOF) {
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
