#include "fsm.h"

void FSM::update() {
    if (state != nullptr) {
        state->exectue();
    }
}


void FSM::set_state(AbstractState* new_state) {
    if (state != nullptr) {
        state->end();
    }

    state = new_state;

    if (state != nullptr) {
        state->start();
    }
}