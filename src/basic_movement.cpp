#include "basic_movement.h"
#include <InputEventMouseMotion.hpp>
#include <InterpolatedCamera.hpp>
#include <KinematicCollision.hpp>
#include <RayCast.hpp>
#include <Label.hpp>
#include <SceneTree.hpp>

using namespace godot;

void BasicMovement::_register_methods() {
    register_method("_input", &BasicMovement::_input, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_ready", &BasicMovement::_ready, GODOT_METHOD_RPC_MODE_DISABLED);
	register_method("init", &BasicMovement::init, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_process", &BasicMovement::_process, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_physics_process", &BasicMovement::_physics_process, GODOT_METHOD_RPC_MODE_DISABLED);

    // physics
    register_property<BasicMovement, float>("max_falling_speed", &BasicMovement::max_falling_speed, -100.0f);
    register_property<BasicMovement, float>("max_glide_fall_speed", &BasicMovement::max_glide_fall_speed, -3.0f);
    register_property<BasicMovement, float>("gravity", &BasicMovement::gravity, -10.0f);

    // camera properties
    register_property<BasicMovement, float>("horiz_camera_sensitivity", &BasicMovement::horiz_camera_sensitivity, 6.0f);
    register_property<BasicMovement, float>("vert_camera_sensitivity", &BasicMovement::vert_camera_sensitivity, 6.0f);
    register_property<BasicMovement, bool>("invert_x_axis", &BasicMovement::invert_x_axis, false);
    register_property<BasicMovement, bool>("invert_y_axis", &BasicMovement::invert_y_axis, false);

    // movement
    register_property<BasicMovement, float>("movement_speed", &BasicMovement::movement_speed, 8.0f);
    register_property<BasicMovement, bool>("ad_rotate", &BasicMovement::ad_rotate, false);
	register_property<BasicMovement, float>("ad_rotate_speed", &BasicMovement::ad_rotate_speed, 3.14f);
    
	register_property<BasicMovement, float>("jump_velocity", &BasicMovement::jump_velocity, 16.0f);
    register_property<BasicMovement, float>("air_control", &BasicMovement::air_control, 1.0f);
    
    register_property<BasicMovement, float>("ledge_stop_test_distance", &BasicMovement::ledge_stop_test_distance, 0.65f);
    register_property<BasicMovement, float>("ledge_grab_distance", &BasicMovement::ledge_grab_distance, 2.0f);
    register_property<BasicMovement, bool>("can_ledge_hang", &BasicMovement::can_ledge_hang, true);

    register_property<BasicMovement, float>("walk_speed", &BasicMovement::walk_speed, 8.0);
    register_property<BasicMovement, float>("sprint_speed", &BasicMovement::sprint_speed, 32.0);
    register_property<BasicMovement, float>("glide_speed", &BasicMovement::glide_speed, 16.0);
    register_property<BasicMovement, float>("walkable_angle", &BasicMovement::walkable_angle, 0.685398);

	// network
	register_property<BasicMovement, Vector3>("slave_position", &BasicMovement::slave_position, Vector3(), GODOT_METHOD_RPC_MODE_PUPPET);
	register_property<BasicMovement, Vector3>("slave_motion", &BasicMovement::slave_motion, Vector3(), GODOT_METHOD_RPC_MODE_PUPPET);
    register_property<BasicMovement, Vector3>("slave_forward", &BasicMovement::slave_forward, Vector3(0, 0, 1), GODOT_METHOD_RPC_MODE_PUPPET);
}

BasicMovement::BasicMovement() {
}

BasicMovement::~BasicMovement() {
}

void BasicMovement::_init() {
	falling_speed = max_falling_speed;
	acceleration = Vector3{0, 0, 0};
	
	forward = Vector3{0, 0, 1};
	right = Vector3{-1, 0, 0};

	mouse_delta = Vector2{0, 0};

	ledge_grab_cooldown = 1.0f;
}

void BasicMovement::_ready() {
	Object::cast_to<RayCast>(get_node("CameraOrientation/RayCast"))->add_exception(get_node("CollisionShape"));
}

void BasicMovement::init(String nickname, Vector3 start_position, bool is_slave) {
	//Object::cast_to<Label>(get_node("GUI/Nickname"))->set_text(nickname);
	set_translation(start_position);
    Godot::print((String) start_position);
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
	if (is_network_master()) {
		update_camera(delta);
		rset_unreliable("slave_forward", forward);
			
		if (ledge_grab_cooldown > 0)
			ledge_grab_cooldown -= delta;
	} else {
		forward = slave_forward;
		right = forward.cross(Vector3{0, 1, 0});
	}
	rotate_player();

}

void BasicMovement::_physics_process(float delta) {
	if (is_network_master()) {
		update_camera_target();
		update_movement(delta);
		move_and_slide(motion, Vector3(0, 1, 0), true, 4, walkable_angle);
		acceleration = Vector3{0, 0, 0};

		rset_unreliable("slave_position", get_translation());
		rset_unreliable("slave_motion", motion);
	} else {
		motion = slave_motion;
		move_and_slide(motion, Vector3(0, 1, 0), true, 4, walkable_angle);
		set_translation(slave_position);
	}

	if(get_tree()->is_network_server()) {
        get_node("/root/network")->call("update_position", get_name().to_int(), get_translation());
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

	if (!ad_rotate && state != LEDGE_HANGING) {
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

void BasicMovement::update_camera_target() {
	auto camera = Object::cast_to<InterpolatedCamera>(get_node("CameraOrientation/RayCast/Camera"));

	auto camera_collision_test = Object::cast_to<RayCast>(get_node("CameraOrientation/RayCast"));

	if (camera_collision_test->is_colliding()) {
		auto target = Object::cast_to<Spatial>(get_node("CameraOrientation/RayCast/CollisionTarget"));

		auto global_target_pos = camera_collision_test->get_collision_point();
		global_target_pos += camera_collision_test->get_collision_normal() * 0.5f;

		target->set_translation(target->get_parent_spatial()->to_local(global_target_pos));
		camera->set_target(target);
	} else {
		camera->set_target(get_node("CameraOrientation/RayCast/RegularTarget"));
	}
}

void BasicMovement::update_movement(float delta) {
	Input* i = Input::get_singleton();

	auto ledge_stop_test = Object::cast_to<RayCast>(get_node("LedgeStopTest/GroundTest"));
	auto ledge_hang_solid_test = Object::cast_to<RayCast>(get_node("MeshParent/LedgeHangTest/SolidTest"));
	auto ledge_hang_air_test = Object::cast_to<RayCast>(get_node("MeshParent/LedgeHangTest/AirTest"));
	auto ledge_hang_ground_test = Object::cast_to<RayCast>(get_node("MeshParent/LedgeHangTest/GroundTest"));

	// state update
	if (is_on_floor()) {
		ledge_stop_test->set_enabled(true);
		ledge_hang_solid_test->set_enabled(false);
		ledge_hang_air_test->set_enabled(false);
		ledge_hang_ground_test->set_enabled(false);
		
		if (state == JUMP || state == FALL || state == GLIDING || state == LEDGE_HANGING) {
			falling_speed = max_falling_speed;
			state = GROUNDED;
			ledge_grab_cooldown = 0;
		}
	} else {
		ledge_stop_test->set_enabled(false);
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
	
	if (state == FALL && i->is_action_pressed("glide") && !sprinting) {
		falling_speed = max_glide_fall_speed;
		movement_speed = glide_speed;
		motion.y = 0;
		state = GLIDING;
	} else if (state == GLIDING && !i->is_action_pressed("glide")) {
		falling_speed = max_falling_speed;
		movement_speed = walk_speed;
		state = FALL;
	}
	if (state == GLIDING && ledge_hang_ground_test->is_colliding()) {
		falling_speed = max_falling_speed;
		movement_speed = walk_speed;
		state = FALL;
	}

	if (!sprinting && i->is_action_pressed("sprint") && state != GLIDING) {
		movement_speed = sprint_speed;
		sprinting = true;
	} else if (sprinting && !i->is_action_pressed("sprint")) {
		movement_speed = walk_speed;
		sprinting = false;
	}

	// motion update
	motion += acceleration * delta;
	
	// input
	int input_x = 0;
	int input_z = 0;
	if (state == LEDGE_HANGING) {
		motion.x = 0.0;
		motion.z = 0.0;
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
			input_z++;
		}
		if (i->is_action_pressed("ui_down")) {
			input_z--;
		}
		if (i->is_action_pressed("ui_left")) {
			input_x--;
		}
		if (i->is_action_pressed("ui_right")) {
			input_x++;
		}

		if (ad_rotate && input_x != 0) {
			float theta = atan2(forward.x, forward.z);
			theta -= input_x * ad_rotate_speed * delta;
			forward = Vector3{(float) sin((double) theta), 0, (float) cos((double) theta)};
			right = Vector3{(float) -cos((double) theta), 0, (float) sin((double) theta)};

			input_x = 0;
		}

		Vector3 perfect_motion = motion;
		perfect_motion.x = 0;
		perfect_motion.z = 0;
		if (input_z != 0) {
			perfect_motion += input_z * movement_speed * forward;
		}
		if (input_x != 0) {
			perfect_motion += input_x * movement_speed * right;
		}

		if (state == GROUNDED) {
			motion = perfect_motion;
		} else {
			motion = motion.linear_interpolate(perfect_motion, air_control);
		}
	}

	// jumping
	if (state == GROUNDED && i->is_action_pressed("ui_select")) {
		motion.y = jump_velocity;
		state = JUMP;
		if (ledge_grab_cooldown < 0.3f)
			ledge_grab_cooldown = 0.3f;
	}
	if (state == JUMP && (!i->is_action_pressed("ui_select") || motion.y <= 0)) {
		state = FALL;
	}

	// ledge stopping
	Vector3 horiz_movement = motion;
	horiz_movement.y = 0;
	Vector3 prev = last_ledge_stop_horiz_movement;
	if (!(horiz_movement.x == 0 && horiz_movement.z == 0)) {
		horiz_movement.normalize();
		last_ledge_stop_horiz_movement = horiz_movement;
	}
	Object::cast_to<Spatial>(get_node("LedgeStopTest"))->set_translation(horiz_movement * ledge_stop_test_distance);
	
	if (state == GROUNDED && !sprinting && !(horiz_movement.x == 0 && horiz_movement.z == 0)) {
		ledge_stop_test->force_raycast_update();
		if (!ledge_stop_test->is_colliding()) {
			motion.x = 0;
			motion.z = 0;
			
			if (prev.dot(last_ledge_stop_horiz_movement) >= 0.9
				&& ((input_z == 1 && i->is_action_just_pressed("ui_up"))
				|| (input_z == -1 && i->is_action_just_pressed("ui_down"))
				|| (input_x == -1 && i->is_action_just_pressed("ui_left"))
				|| (input_x == 1 && i->is_action_just_pressed("ui_right")))) {
				
				// ledge hang
				Godot::print("ledge hang");

				float ledge_distance = (2.0f - ledge_stop_test_distance) + CMP_EPSILON;
				constexpr float vert_distance = -2.7f;

				set_translation(get_translation() + horiz_movement * ledge_distance + Vector3 {0, vert_distance, 0});
				state = LEDGE_HANGING;
				forward = -horiz_movement;
				right = forward.cross(Vector3{0, 1, 0});
				motion.y = 0;
			}
		}
	}

	// ledge hanging
	if (!is_on_floor() && (state == FALL || state == JUMP || state == GLIDING) && ledge_grab_cooldown <= 0) {
		if (ledge_hang_solid_test->is_colliding() && !ledge_hang_air_test->is_colliding() && !ledge_hang_air_test->is_colliding()) {
			state = LEDGE_HANGING;
			motion = Vector3 {0, 0, 0};
		}
	}

	// motion clamping
	if (motion.y < falling_speed) {
		motion.y = falling_speed;
	}

	if (i->is_action_just_pressed("ui_squirrel")){
		AudioStreamPlayer3D *a1 = (AudioStreamPlayer3D *) get_node("/root/Game/Player/GreatSound");
		a1->play();
	}

	if (i->is_action_just_pressed("ui_rotate")){
		if(ad_rotate)
			ad_rotate = false;
		else 
			ad_rotate = true;
	}
}

// ad_rotate variable is not changed actually
void BasicMovement::toggle_AD_rotate () {

	if(ad_rotate)
		ad_rotate = false;
	else 
		ad_rotate = true;
}
