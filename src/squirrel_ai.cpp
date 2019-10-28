#include "squirrel_ai.h"

using namespace godot;

void SquirrelAI::_register_methods() {
	register_method("_ready", &SquirrelAI::_ready);
    register_method("_process", &SquirrelAI::_process);
    register_method("_physics_process", &SquirrelAI::_physics_process);
    register_method("_rotate_player", &SquirrelAI::_rotate_player);
    register_method("_update_movement", &SquirrelAI::_update_movement);
    register_method("_get_closest_node", &SquirrelAI::_get_closest_node);
}

SquirrelAI::SquirrelAI() {
}

SquirrelAI::~SquirrelAI() {
}

void SquirrelAI::_init() {
    brain.set_state(&wanderState);
}

void SquirrelAI::_ready() {
}

void SquirrelAI::_process(float delta) {
    brain.update();
}


void SquirrelAI::_physics_process(float delta) {
    _update_movement(delta);
    move_and_slide(motion, Vector3(0, 1, 0), true, 4, 1.5);
}

void SquirrelAI::_rotate_player() {
    
}

void SquirrelAI::_update_movement(float delta) {
    
}

Waypoint* SquirrelAI::_get_closest_node () {
    Node* waypoint_parent = get_tree()->get_root()->get_node("GameState");
    //Node* waypoint_parent = find_node("GameState", true, false);
    Godot::print(get_tree()->get_root()->get_node("GameState")->get_name());
    
    //auto children = waypoint_parent->get_children();
    /*
    Spatial* child_spatial = Object::cast_to<Spatial>(children[0]);
    Spatial* this_spatial = Object::cast_to<Spatial>(this);
    float min_dist = (this_spatial->get_translation()).distance_squared_to(child_spatial->get_translation());
    int min_index = 0;
    for (int i = 1; i < children.size(); ++i) {
        child_spatial = Object::cast_to<Spatial>(children[i]);
        float new_dist = (this_spatial->get_translation()).distance_squared_to(child_spatial->get_translation());
        if (new_dist < min_dist) {
            min_dist = new_dist;
            min_index = i;
        }
    }
    */
   return nullptr;
    //return Object::cast_to<Waypoint>(children[min_index]);
}

void WanderState::start() {
    Godot::print("start wandering");
    //current_waypoint = _get_closest_node();
}

void WanderState::execute() {
    Godot::print("execute wandering");

    // if (current_waypoint != nullptr) {
    //     Vector3 temp_motion = Object::cast_to<Spatial>(current_waypoint)->get_translation() - Object::cast_to<Spatial>(this)->get_translation();
    //     motion = temp_motion.normalized() * wander_speed;
    // }
}

void WanderState::end() {
    Godot::print("end wander");
}
