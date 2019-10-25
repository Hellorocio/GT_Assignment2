#include "AI/squirrel_ai.h"

using namespace godot;

void SquirrelAI::_register_methods() {
	register_method("_ready", &SquirrelAI::_ready);
    register_method("_process", &SquirrelAI::_process);
}

SquirrelAI::SquirrelAI() {
}

SquirrelAI::~SquirrelAI() {
}

void SquirrelAI::_init() {
}

void SquirrelAI::_ready() {
}

void SquirrelAI::_process(float delta) {

}



