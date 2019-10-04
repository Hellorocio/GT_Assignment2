#include "basic_movement.h"
#include <InputEventMouseMotion.hpp>

using namespace godot;

void BasicMovement::_register_methods() {
    register_method("_input", &BasicMovement::_input);
    register_method("_process", &BasicMovement::_process);
    register_method("_physics_process", &BasicMovement::_physics_process);
    register_method("update_movement", &BasicMovement::update_movement);

    // physics
    register_property<BasicMovement, float>("gravity", &BasicMovement::gravity, -10.0f);

    // camera properties
    register_property<BasicMovement, float>("horiz_camera_sensitivity", &BasicMovement::horiz_camera_sensitivity, 6.0f);
    register_property<BasicMovement, float>("vert_camera_sensitivity", &BasicMovement::vert_camera_sensitivity, 6.0f);
    register_property<BasicMovement, bool>("invert_x_axis", &BasicMovement::invert_x_axis, false);
    register_property<BasicMovement, bool>("invert_y_axis", &BasicMovement::invert_y_axis, false);

    // movement
    register_property<BasicMovement, float>("movement_speed", &BasicMovement::movement_speed, 8.0f);
    register_property<BasicMovement, bool>("adRotate", &BasicMovement::adRotate, false);
}

BasicMovement::BasicMovement() {
}

BasicMovement::~BasicMovement() {
    // add your cleanup here
}

void BasicMovement::_init() {
    // initialize any variables here
	falling_speed_max = -100.0;
	acceleration = Vector3{0, 0, 0};
	
	forward = Vector3{0, 0, 1};
	right = Vector3{-1, 0, 0};

	mouse_delta = Vector2{0, 0};
}

void BasicMovement::_input(InputEvent *event) {
	auto mouse_event = Object::cast_to<InputEventMouseMotion>(event);
	if (mouse_event == nullptr)
		return;

	auto delta = mouse_event->get_relative();
	if (!(delta.x == 0 && delta.y == 0))
		mouse_delta = delta;
}

void BasicMovement::_process(float delta) {
	update_camera(delta);
	rotate_player();
}

void BasicMovement::_physics_process(float delta) {
	update_movement(delta);
	move_and_slide(motion, Vector3(0, 1, 0), true, 4, 0.685398);
	acceleration = Vector3{0, 0, 0};
} 

void BasicMovement::update_camera(float delta) {
	constexpr float PI = Math_PI;

	Input* i = Input::get_singleton();

	if (!i->is_action_pressed("camera_move")) {
		i->set_mouse_mode(Input::MouseMode::MOUSE_MODE_VISIBLE);
		return;
	}
	i->set_mouse_mode(Input::MouseMode::MOUSE_MODE_CAPTURED);

	Vector2 mouse_vel = Vector2{-mouse_delta.x, mouse_delta.y};

	yaw += mouse_vel.x * horiz_camera_sensitivity * delta;
	pitch += mouse_vel.y * vert_camera_sensitivity * delta;
	if (pitch < -PI / 6.0f)
		pitch = -PI / 6.0f;
	else if (pitch > PI / 2.0f)
		pitch = PI / 2.0f;

	forward = Vector3{(float) sin((double) yaw), 0, (float) cos((double) yaw)};
	right = Vector3{(float) -cos((double) yaw), 0, (float) sin((double) yaw)};

	
	Node *node = get_node("CameraOrientation");
	if (node != nullptr) {
		auto orientation_node = Object::cast_to<Spatial>(node);
		if (orientation_node != nullptr) {
			orientation_node->set_transform(Transform(Basis(Vector3{pitch, yaw, 0}), orientation_node->get_translation()));
		}
	}
	
	mouse_delta = Vector2{0, 0};
}

void BasicMovement::rotate_player() {
	Node *node = get_node("MeshParent");
	if (node != nullptr) {
		auto orientation_node = Object::cast_to<Spatial>(node);
		if (orientation_node != nullptr) {
			orientation_node->set_transform(Transform(Basis(right, Vector3{0, 1, 0}, forward), orientation_node->get_translation()));
		}
	}
}

void BasicMovement::update_movement(float delta) {
	Input* i = Input::get_singleton();

	// state update
	if (is_on_floor()) {
		if (state == JUMP || state == FALL || state == GLIDING) {
			falling_speed_max = -100.0;
			state = GROUNDED;
		}
	} else {
		if (state == GROUNDED) {
			state = FALL;
		}
		
		acceleration.y += gravity;
	}
	
	if (state == FALL && i->is_action_pressed("glide")) {
		falling_speed_max = -3.0;
		motion.y = 0;
		state = GLIDING;
	} else if (state == GLIDING && !i->is_action_pressed("glide")) {
		falling_speed_max = -100.0;
		state = FALL;
	}

	// motion update
	motion.x = 0.0;
	motion.z = 0.0;

	motion += acceleration * delta;
	
	// input
	if (i->is_action_pressed("ui_up")) {
		motion += forward * movement_speed;
	}
	if (i->is_action_pressed("ui_down")) {
		motion -= forward * movement_speed;
	}
	if (i->is_action_pressed("ui_left")) {
		if (!adRotate) {
			motion -= right * movement_speed;
		} else {
			//TODO: rotate player left ()
			//Basis rotation = transform.basis.rotated(Vector3(1, 0, 0), PI);
			//rotate(Vector3(0, -0.1, 0), Math_PI);
		}
		
	}
	if (i->is_action_pressed("ui_right")) {
		if (!adRotate) {
			motion += right * movement_speed;
		} else {
			//TODO: rotate player right
			//Godot::print("Trying to rotate right");
			//this->rotate(Vector3(0, 0.1, 0), Math_PI);
		}
	}

	if (state == GROUNDED && i->is_action_pressed("ui_select")) {
		motion.y = 16.0;
		state = JUMP;
	}
	if (state == JUMP && (!i->is_action_pressed("ui_select") || motion.y <= 0)) {
		state = FALL;
	}

	// motion clamping
	if (motion.y < falling_speed_max) {
		motion.y = falling_speed_max;
	}
}
