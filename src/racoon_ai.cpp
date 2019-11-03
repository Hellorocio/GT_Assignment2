#include "racoon_ai.h"

using namespace godot;

void ChaseState::start(Node* parent) {
    
}

void ChaseState::execute(Node* parent, float dt)
{
    auto current = parent->get_node(current_target);
    auto current_rotation = parent->get_node(current_target_rotation);

    if (current != nullptr) {
        // move squirrel towards current acorn
        Vector3 delta = Object::cast_to<Spatial>(current)->get_translation() - Object::cast_to<Spatial>(parent)->get_translation();
        Vector3 forward_parent = Object::cast_to<Spatial>(parent)->get_global_transform().basis.x;
        Vector3 forward_current = Object::cast_to<Spatial>(current_rotation)->get_global_transform().basis.x;

        float rotation_delta = forward_parent.dot(forward_current);
        Godot::print(String::num(rotation_delta));
        if (rotation_delta <= -0.9) {
            // change state to run away if squirrel faces the racoon
            Object::cast_to<RacoonAI>(parent)->runAwayState.current_waypoint = Object::cast_to<BaseAI>(parent)->get_farthest_node_to_point(Object::cast_to<Spatial>(current)->get_translation());
            Object::cast_to<BaseAI>(parent)->brain.set_state(parent, &(Object::cast_to<RacoonAI>(parent)->runAwayState));
        }

        float len = delta.length();
        Vector3 normalized_delta = delta * 6 / len;
        if (len < 1)
            normalized_delta = delta * 6;        

        Object::cast_to<BaseAI>(parent)->_update_movement(normalized_delta, dt);
        Object::cast_to<BaseAI>(parent)->_turn_to_face(Object::cast_to<Spatial>(current)->get_translation());
    } else {
        // switch back to wanderState
         Object::cast_to<BaseAI>(parent)->brain.set_state(parent, &(Object::cast_to<RacoonAI>(parent)->wanderState));
    }
}

void ChaseState::end(Node* parent) {

}

void RunAwayState::start(Node* parent) {
    Godot::print("Racoon: Run Away start");

    path = Object::cast_to<BaseAI>(parent)->calculate_shortest_path(parent->get_path(), current_waypoint);
    Object::cast_to<BaseAI>(parent)->wander_speed *= 2;
}

void RunAwayState::execute(Node* parent, float dt) {
    Node* waypoint_parent = parent->get_node("/root/Game/Waypoints");

    BaseAI* parent_ai = Object::cast_to<BaseAI>(parent);

    if (path.empty()) {
        // go to wander state
        Object::cast_to<BaseAI>(parent)->brain.set_state(parent, &(Object::cast_to<RacoonAI>(parent)->wanderState));
           
    } else {
        Vector3 target = path.back();
        Vector3 movement = parent_ai->get_movement_vector_to_target(target, dt);
        if (movement == Vector3{}) {
            path.pop_back();
            if (!path.empty()) {
                parent_ai->_turn_to_face((Vector3) path.back());
            }
        } else {
            parent_ai->_update_movement(movement, dt);
        }
    }
}

void RunAwayState::end(Node* parent) {
    Object::cast_to<BaseAI>(parent)->wander_speed /= 2;  
}

void RacoonAI::_register_methods() {
	register_method("_ready", &RacoonAI::_ready);
    register_method("_on_body_entered", &RacoonAI::_on_body_entered);
}

RacoonAI::RacoonAI() {
}

RacoonAI::~RacoonAI() {
}

void RacoonAI::_init() {
}

void RacoonAI::_ready() {
    brain.set_state(this, &wanderState);

    get_node("RaccoonVision")->connect("body_entered", this, "_on_body_entered");
}

void RacoonAI::_on_body_entered (Node* body) {
    if (brain.state != &chaseState && (body->get_name() == "SquirrelFriend" || body->get_name() == "1")) {
        chaseState.current_target = body->get_path();
        chaseState.current_target_rotation = body->get_path();
        if (body->get_name() == "1") {
            chaseState.current_target_rotation = body->get_node("MeshParent")->get_path();
        }
        brain.set_state(this, &chaseState);
    }
    
}


