#include "basic_movement.h"
#include <InputEventMouseMotion.hpp>
#include <RayCast.hpp>

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

    register_property<BasicMovement, float>("ledge_grab_distance", &BasicMovement::ledge_grab_distance, 2.0f);
    register_property<BasicMovement, bool>("can_ledge_hang", &BasicMovement::can_ledge_hang, true);
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

	ledge_grab_cooldown = 1.0f;
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

	if (ledge_grab_cooldown > 0)
		ledge_grab_cooldown -= delta;
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

	if (state != LEDGE_HANGING) {
		forward = Vector3{(float) sin((double) yaw), 0, (float) cos((double) yaw)};
		right = Vector3{(float) -cos((double) yaw), 0, (float) sin((double) yaw)};
	}

	
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

	auto ledge_hang_solid_test = Object::cast_to<RayCast>(get_node("MeshParent/LedgeHangTest/SolidTest"));
	auto ledge_hang_air_test = Object::cast_to<RayCast>(get_node("MeshParent/LedgeHangTest/AirTest"));
	auto ledge_hang_ground_test = Object::cast_to<RayCast>(get_node("MeshParent/LedgeHangTest/GroundTest"));

	// state update
	if (is_on_floor()) {
		ledge_hang_solid_test->set_enabled(false);
		ledge_hang_air_test->set_enabled(false);
		ledge_hang_ground_test->set_enabled(false);
		
		if (state == JUMP || state == FALL || state == GLIDING || state == LEDGE_HANGING) {
			falling_speed_max = -100.0;
			state = GROUNDED;
			ledge_grab_cooldown = 0;
		}
	} else {
		ledge_hang_solid_test->set_enabled(true);
		ledge_hang_air_test->set_enabled(true);
		ledge_hang_ground_test->set_enabled(true);

		if (state == GROUNDED) {
			state = FALL;
		}
		
		if (state != LEDGE_HANGING) {
			acceleration.y += gravity;
		}
	}
	
	if (state == FALL && i->is_action_pressed("glide")) {
		falling_speed_max = -3.0;
		motion.y = 0;
		state = GLIDING;
	} else if (state == GLIDING && !i->is_action_pressed("glide")) {
		falling_speed_max = -100.0;
		state = FALL;
	}

	if (!sprinting && i->is_action_pressed("sprint")) {
		movement_speed = 32.0;
		sprinting = true;
	} else if (sprinting && !i->is_action_pressed("sprint")) {
		movement_speed = 8.0;
		sprinting = false;
	}

	// motion update
	motion.x = 0.0;
	motion.z = 0.0;

	motion += acceleration * delta;
	
	// input
	if (state == LEDGE_HANGING) {
		if (i->is_action_just_pressed("ui_up") || i->is_action_just_pressed("ui_select")) {
			state = JUMP;
			motion.y = 16.0;
			ledge_grab_cooldown = 1;
		} else if (i->is_action_just_pressed("ui_down")) {
			state = FALL;
			ledge_grab_cooldown = 1;
		}
	} else {
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
				// forward = Vector3{(float) sin((double) yaw), 0, (float) cos((double) yaw)};
				// right = Vector3{(float) -cos((double) yaw), 0, (float) sin((double) yaw)};
			}
			
		}
		if (i->is_action_pressed("ui_right")) {
			if (!adRotate) {
				motion += right * movement_speed;
			} else {
				//TODO: rotate player right
				//Godot::print("Trying to rotate right");
				// forward = Vector3{(float) sin((double) yaw), 0, (float) cos((double) yaw)};
				// right = Vector3{(float) -cos((double) yaw), 0, (float) sin((double) yaw)};
			}
		}
	}

	if (state == GROUNDED && i->is_action_pressed("ui_select")) {
		motion.y = 16.0;
		state = JUMP;
		if (ledge_grab_cooldown < 0.3f)
			ledge_grab_cooldown = 0.3f;
	}
	if (state == JUMP && (!i->is_action_pressed("ui_select") || motion.y <= 0)) {
		state = FALL;
	}

	// ledge hanging
	if (!is_on_floor() && (state == FALL || state == JUMP || state == GLIDING) && ledge_grab_cooldown <= 0) {
		if (ledge_hang_solid_test->is_colliding() && !ledge_hang_air_test->is_colliding() && !ledge_hang_air_test->is_colliding()) {
			state = LEDGE_HANGING;
			motion = Vector3 {0, 0, 0};
		}
	} else if (state == LEDGE_HANGING) {
		if (!ledge_hang_solid_test->is_colliding() || ledge_hang_air_test->is_colliding() || ledge_hang_air_test->is_colliding()) {
			state = FALL;
			ledge_grab_cooldown = 1;
		}
	}

	// motion clamping
	if (motion.y < falling_speed_max) {
		motion.y = falling_speed_max;
	}
}
