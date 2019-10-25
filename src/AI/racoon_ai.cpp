#include "AI/racoon_ai.h"

using namespace godot;

void RacoonAI::_register_methods() {
	register_method("_ready", &RacoonAI::_ready);
    register_method("_process", &RacoonAI::_process);
}

RacoonAI::RacoonAI() {
}

RacoonAI::~RacoonAI() {
}

void RacoonAI::_init() {
}

void RacoonAI::_ready() {
}

void RacoonAI::_process(float delta) {

}



