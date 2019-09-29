#include "basic_movement.h"
#include <InputEventMouseMotion.hpp>

using namespace godot;

void BasicMovement::_register_methods() {
    register_method("_input", &BasicMovement::_input);
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

    //gravity
    register_property<BasicMovement, float>("gravity", &BasicMovement::movement_speed, -10.0f);
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
	jump_frame = 0;

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
}

void BasicMovement::_physics_process(float delta) {
	update_movement_from_input(delta);
	move_and_slide(motion, Vector3(0, 1, 0));
	if (is_on_floor()){
		falling_speed = 0;
	}

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

void BasicMovement::update_movement_from_input(float delta) {
	//motion = Vector3(0.0, 0.0, 0.0);
	motion.x = 0.0;
	motion.z = 0.0;
	if (falling_speed <= 5.0){
		printf("%g\n",delta);
		falling_speed += delta * gravity;
	}
	motion.y += falling_speed;
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
	if (i->is_action_pressed("ui_select") && is_on_floor()) {
		motion.y = 20.0;
	}

	//translate(motion);
}