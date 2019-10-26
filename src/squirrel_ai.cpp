#include "squirrel_ai.h"

using namespace godot;

void SquirrelAI::_register_methods() {
	register_method("_ready", &SquirrelAI::_ready);
    register_method("_process", &SquirrelAI::_process);
    register_method("_physics_process", &SquirrelAI::_physics_process);
    register_method("_rotate_player", &SquirrelAI::_rotate_player);
    register_method("_update_movement", &SquirrelAI::_update_movement);
}

SquirrelAI::SquirrelAI() {
}

SquirrelAI::~SquirrelAI() {
    //delete(wanderState);
    //delete(brain);
}

void SquirrelAI::_init() {
    //brain->set_state(wanderState);
}

void SquirrelAI::_ready() {
}

void SquirrelAI::_process(float delta) {
    //brain->update();
}

void SquirrelAI::_physics_process(float delta) {

}

void SquirrelAI::_rotate_player() {
    
}

void SquirrelAI::_update_movement(float delta) {
    
}


Wander::Wander() {

}

Wander::~Wander() {
    
}

void Wander::start() {
    Godot::print("start wandering");
}

void Wander::execute() {
    Godot::print("execute wandering");
}

void Wander::end() {

}
