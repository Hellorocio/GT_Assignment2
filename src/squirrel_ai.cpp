#include "squirrel_ai.h"

using namespace godot;

WanderState::WanderState(Node* squirrel) {
    squirrelAI = squirrel;
}

void WanderState::start() {
    //Godot::print("start wandering");
    // TODO: this call is not working
    //current_waypoint = squirrelAI->call("_get_closest_node");
}

void WanderState::execute() {
    //Godot::print("execute wandering");

    if (current_waypoint != nullptr) {
        Godot::print("execute wandering");
        Vector3 temp_motion = Object::cast_to<Spatial>(current_waypoint)->get_translation() - Object::cast_to<Spatial>(squirrelAI)->get_translation();
        squirrelAI->call("_update_movement", temp_motion.normalized() * 2);
    }
}

void WanderState::end() {
    //Godot::print("end wander");
}

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
    delete(wanderState);
}

void SquirrelAI::_init() {
    
}

void SquirrelAI::_ready() {
    wanderState = new WanderState(get_parent()->get_node(this->get_name()));
    brain.set_state(wanderState);
}

void SquirrelAI::_process(float delta) {
    brain.update();
}

void SquirrelAI::_physics_process(float delta) {
    move_and_slide(motion, Vector3(0, 1, 0), true, 4, 1.5);
}

void SquirrelAI::_rotate_player() {
    
}

void SquirrelAI::_update_movement(Vector3 direction) {
    Godot::print("update_movement:");
    Godot::print(direction);
    motion = direction;
}

Waypoint* SquirrelAI::_get_closest_node () {
    Godot::print("_get_closest_node: start");
    Node* waypoint_parent = find_node("Waypoints", true, false);
    
    auto children = waypoint_parent->get_children();
    
    Spatial* child_spatial = Object::cast_to<Spatial>(children[0]);
    Spatial* this_spatial = Object::cast_to<Spatial>(this);
    float min_dist = (this_spatial->get_translation()).distance_squared_to(child_spatial->get_translation());
    int min_index = 0;
    for (int i = 1; i < children.size(); ++i) {
        Godot::print("_get_closest_node");
        child_spatial = Object::cast_to<Spatial>(children[i]);
        float new_dist = (this_spatial->get_translation()).distance_squared_to(child_spatial->get_translation());
        if (new_dist < min_dist) {
            min_dist = new_dist;
            min_index = i;
        }
    }
    return Object::cast_to<Waypoint>(children[min_index]);
}





