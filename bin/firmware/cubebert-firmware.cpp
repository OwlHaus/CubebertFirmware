#include "state.h"
#include "pico/stdio.h"
#include "pico/time.h"

bool init() {
    return true;
}

int main() {
    stdio_init_all();

    // Welcome My Friend, welcome to
    StateMachine theMachine(init() ? State::Ready : State::Shutdown);

    while(theMachine.state() != State::Shutdown) {
        switch(theMachine.state()){
        case State::Ready:
            sleep_ms(10000);
            //printf("State: Ready\n");
            theMachine.Ready();
            continue;
        default:
            theMachine.setState(State::Shutdown);
        }

        // Sleep for 250 between cycles to keep from overflexing the processor 
        sleep_ms(500);
    }
    printf("Why'd we exit? %d", theMachine.state());
}
