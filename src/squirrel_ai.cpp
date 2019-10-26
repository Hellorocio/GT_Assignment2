#include "squirrel_ai.h"

using namespace godot;

void SquirrelAI::_register_methods() {
	register_method("_ready", &SquirrelAI::_ready);
    register_method("_process", &SquirrelAI::_process);
    register_method("_physics_process", &SquirrelAI::_physics_process);
    register_method("_rotate_player", &SquirrelAI::_rotate_player);
    register_method("_update_movement", &SquirrelAI::_update_movement);
    register_method("_change_state", &SquirrelAI::_change_state);
    register_method("_get_closest_node", &SquirrelAI::_get_closest_node);
}

SquirrelAI::SquirrelAI() {
}

SquirrelAI::~SquirrelAI() {
    //delete(wanderState);
    //delete(brain);
}

void SquirrelAI::_init() {
    //brain->set_state(wanderState);
    _change_state(WANDER);
}

void SquirrelAI::_ready() {
}

void SquirrelAI::_process(float delta) {
    //brain->update();
    switch (state)
    {
        case WANDER:
            Godot::print("wandering");
            if (current_waypoint != nullptr) {
                Vector3 temp_motion = Object::cast_to<Spatial>(current_waypoint)->get_translation() - Object::cast_to<Spatial>(this)->get_translation();
                motion = temp_motion.normalized() * wander_speed;
            }
            

            break;

    }
}


void SquirrelAI::_physics_process(float delta) {
    _update_movement(delta);
    move_and_slide(motion, Vector3(0, 1, 0), true, 4, 1.5);
}

void SquirrelAI::_rotate_player() {
    
}

void SquirrelAI::_update_movement(float delta) {
    
}


void SquirrelAI::_change_state(int s) {

    //stop current state
    switch (state)
    {
        case WANDER:
            Godot::print("end wander");
            break;
        default:
            break;

    }
    
    state = static_cast<AIState>(s);

    switch (state)
    {
        case WANDER:
            Godot::print("start wander");
            current_waypoint = _get_closest_node();
            
            break;
        default:
            break;
    }
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

/*
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
*/