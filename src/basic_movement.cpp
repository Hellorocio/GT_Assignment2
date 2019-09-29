#include "basic_movement.h"

using namespace godot;

void BasicMovement::_register_methods() {
    register_method("_process", &BasicMovement::_process);
    register_method("_physics_process", &BasicMovement::_physics_process);
    register_method("update_movement_from_input", &BasicMovement::update_movement_from_input);

    // camera properties
    register_property<BasicMovement, float>("horiz_camera_sensitivity", &BasicMovement::horiz_camera_sensitivity, 6.0f);
    register_property<BasicMovement, float>("vert_camera_sensitivity", &BasicMovement::vert_camera_sensitivity, 6.0f);
    register_property<BasicMovement, bool>("invert_x_axis", &BasicMovement::invert_x_axis, false);
    register_property<BasicMovement, bool>("invert_y_axis", &BasicMovement::invert_y_axis, false);

    // movement
    register_property<BasicMovement, float>("movement_speed", &BasicMovement::movement_speed, 8.0f);
}

BasicMovement::BasicMovement() {
}

BasicMovement::~BasicMovement() {
    // add your cleanup here
}

void BasicMovement::_init() {
    // initialize any variables here
	forward = Vector3{0, 0, 1};
	right = Vector3{-1, 0, 0};

	Input* i = Input::get_singleton();
	//i->set_mouse_mode(Input::MouseMode::MOUSE_MODE_CAPTURED);
}

void BasicMovement::_process(float delta) {
	update_camera(delta);
}

void BasicMovement::_physics_process(float delta) {
	update_movement_from_input();
	move_and_slide(motion);
} 

void BasicMovement::update_camera(float delta) {
	constexpr float PI = Math_PI;

	Input* i = Input::get_singleton();
	Vector2 mouse_vel = i->get_last_mouse_speed();

	yaw += mouse_vel.x * horiz_camera_sensitivity * delta;
	pitch += mouse_vel.y * vert_camera_sensitivity * delta;
	if (pitch < -PI / 6.0f)
		pitch = -PI / 6.0f;
	else if (pitch > PI)
		pitch = PI;

	/*
	forward = Vector3{sin(yaw), 0, cos(yaw)};
	right = Vector3{sin(yaw + PI / 2.0f), 0, cos(yaw + PI / 2.0f)};
	Godot::print((String) forward);


	Node *node = get_node("CameraOrientation");
	if (node != nullptr) {
		auto orientation_node = Object::cast_to<Spatial>(node);
		if (orientation_node != nullptr) {
			//orientation_node->set_rotation();
		}
	}*/
}

void BasicMovement::update_movement_from_input() {
	motion = Vector3(0.0, 0.0, 0.0);
	Input* i = Input::get_singleton();
	if (i->is_action_pressed("ui_up")) {
		motion += forward * movement_speed;
	}
	if (i->is_action_pressed("ui_down")) {
		motion -= forward * movement_speed;
	}
	if (i->is_action_pressed("ui_left")) {
		motion -= right * movement_speed;
	}
	if (i->is_action_pressed("ui_right")) {
		motion += right * movement_speed;
	}
	//translate(motion);
}