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
            Godot::print("Start run away");
            // change state to run away if squirrel faces the racoon
            //Object::cast_to<BaseAI>(parent)->brain.set_state(parent, &(Object::cast_to<RacoonAI>(parent)->runAwayState));
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
}

void RunAwayState::execute(Node* parent, float delta) {

}

void RunAwayState::end(Node* parent) {
    
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


