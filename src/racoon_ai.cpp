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
        if (rotation_delta <= -0.9) {
            // change state to run away if squirrel faces the racoon
            Object::cast_to<RacoonAI>(parent)->runAwayState.current_waypoint = Object::cast_to<BaseAI>(parent)->get_farthest_node_to_point(Object::cast_to<Spatial>(current)->get_translation());
            Object::cast_to<BaseAI>(parent)->brain.set_state(parent, &(Object::cast_to<RacoonAI>(parent)->runAwayState));
        }
        
        float len = delta.length();
        Vector3 normalized_delta = delta / len;
        //Godot::print(String::num(len));
        if (len <= 2.5) {
            // steal acorns
            GameState * state = Object::cast_to<GameState>(parent->get_node("/root/Game/GameState"));
            state->remove_acorn(1);

            //play particle effect
            Particles * acornParticles = Object::cast_to<Particles>(parent->get_node((String)current_target + "/Particles"));
            if (acornParticles) {
                acornParticles->set_emitting(true);
            }

            // change state to run away
            Object::cast_to<RacoonAI>(parent)->runAwayState.current_waypoint = Object::cast_to<BaseAI>(parent)->get_farthest_node_to_point(Object::cast_to<Spatial>(current)->get_translation());
            Object::cast_to<BaseAI>(parent)->brain.set_state(parent, &(Object::cast_to<RacoonAI>(parent)->runAwayState));
        }

               
        
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

    auto parent_ai = Object::cast_to<BaseAI>(parent);
    auto current = parent->get_node("/root/Game/Waypoints")->get_node(current_waypoint);
    path = parent_ai->get_shortest_path(Object::cast_to<Spatial>(parent_ai)->get_translation(), Object::cast_to<Spatial>(current)->get_translation());
    parent_ai->movement_speed *= 1.5;
}

void RunAwayState::execute(Node* parent, float dt) {
    Node* waypoint_parent = parent->get_node("/root/Game/Waypoints");

    BaseAI* parent_ai = Object::cast_to<BaseAI>(parent);

    if (path.empty()) {
        // go to wander state
        Object::cast_to<BaseAI>(parent)->brain.set_state(parent, &(Object::cast_to<RacoonAI>(parent)->wanderState));
           
    } else {
        Vector3 target = path.back();
        bool finished = false;
        Vector3 movement = parent_ai->get_movement_vector_to_target(target, finished);

        parent_ai->_update_movement(movement, dt);
        parent_ai->_turn_to_face(target);

        if (finished) {
            path.pop_back();
        }
    }
}

void RunAwayState::end(Node* parent) {
    Object::cast_to<BaseAI>(parent)->movement_speed /= 1.5;  
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
    if (brain.state == &wanderState && (body->get_name() == "SquirrelFriend" || body->get_name() == "1")) {
        chaseState.current_target = body->get_path();
        chaseState.current_target_rotation = body->get_path();
        if (body->get_name() == "1") {
            chaseState.current_target_rotation = body->get_node("MeshParent")->get_path();
        }
        brain.set_state(this, &chaseState);
    }
}


