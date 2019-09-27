#include "basic_movement.h"

using namespace godot;

void BasicMovement::_register_methods() {
    register_method("_process", &BasicMovement::_process);
    register_method("UpdateMovementFromInput", &BasicMovement::UpdateMovementFromInput);
}

BasicMovement::BasicMovement() {
}

BasicMovement::~BasicMovement() {
    // add your cleanup here
}

void BasicMovement::_init() {
    // initialize any variables here

}

void BasicMovement::_process(float delta) {
	UpdateMovementFromInput();
	move_and_slide(motion);
}

void BasicMovement::UpdateMovementFromInput(){
	motion = Vector3(0.0, 0.0, 0.0);
	Input* i = Input::get_singleton();
	if (i->is_action_pressed("ui_up")) {
		motion.z -= 1;
	}
	if (i->is_action_pressed("ui_down")) {
		motion.z += 1;
	}
	if (i->is_action_pressed("ui_left")) {
		motion.x -= 1;
	}
	if (i->is_action_pressed("ui_right")) {
		motion.x += 1;
	}
	//translate(motion);
}