#include "squirrel_ai.h"

using namespace godot;


void FoundAcorn::start(Node* parent) {
    //Godot::print("FoundAcorn: start");
}

void FoundAcorn::execute(Node* parent, float dt) {
    auto current = parent->get_node(current_acorn);

    if (current != nullptr) {
        // move squirrel towards current acorn
        Vector3 target = Object::cast_to<Spatial>(current)->get_translation();
        bool finished = false;
        Vector3 movement = Object::cast_to<BaseAI>(parent)->get_movement_vector_to_target(target, finished);       

        Object::cast_to<BaseAI>(parent)->_update_movement(movement, dt);
        Object::cast_to<BaseAI>(parent)->_turn_to_face(target);

        if (finished) {
            current = nullptr;
        }
    } else {
        // switch back to wanderState
        Object::cast_to<BaseAI>(parent)->brain.set_state(parent, &(Object::cast_to<SquirrelAI>(parent)->wanderState));
    }
}

void FoundAcorn::end(Node* parent) {

}

void ScareRacoon::start(Node* parent) {
     Object::cast_to<BaseAI>(parent)->_update_movement(Vector3(0, 0, 0), 0);
}

void ScareRacoon::execute(Node* parent, float dt) {
    auto current = parent->get_node(racoon);

    if (current != nullptr) {
        Object::cast_to<BaseAI>(parent)->_turn_to_face(Object::cast_to<Spatial>(current)->get_translation());

        Vector3 delta = Object::cast_to<Spatial>(current)->get_translation() - Object::cast_to<Spatial>(parent)->get_translation();
        if (delta.length() >= 30)
        {
            // switch back to wander
            Object::cast_to<BaseAI>(parent)->brain.set_state(parent, &(Object::cast_to<SquirrelAI>(parent)->wanderState));
        }
        
    }

}

void ScareRacoon::end(Node* parent) {
    
}

void SquirrelAI::_register_methods() {
	register_method("_ready", &SquirrelAI::_ready);
    register_method("_rotate_player", &SquirrelAI::_rotate_player);
    register_method("_on_area_entered", &SquirrelAI::_on_area_entered);
    register_method("_on_body_entered", &SquirrelAI::_on_body_entered);
}



SquirrelAI::SquirrelAI() {
}

SquirrelAI::~SquirrelAI() {
}

void SquirrelAI::_init() {
    
}

void SquirrelAI::_ready() {
    brain.set_state(this, &wanderState);

    get_node("SquirrelVision")->connect("area_entered", this, "_on_area_entered");
    get_node("SquirrelVision")->connect("body_entered", this, "_on_body_entered");
}

void SquirrelAI::_rotate_player() {
    
}

void SquirrelAI::_on_area_entered (Area* area) {
    Node* areaNode = (Node*)area;
	String name = areaNode->get_name();
    
    if (areaNode->get_name() == "AcornArea" && brain.state == &wanderState) {
        //Godot::print("area_entered signal: found acorn");
        // change state to FoundAcorn if not already in FoundAcorn
        foundAcorn.current_acorn = areaNode->get_parent()->get_path();
        brain.set_state(this, &foundAcorn);
    }
}

void SquirrelAI::_on_body_entered (Node* body) {
    
    if (body->get_name() == "Racoon" && brain.state != &scareRacoon) {
        //Godot::print("area_entered signal: found acorn");
        // change state to FoundAcorn if not already in FoundAcorn
        scareRacoon.racoon = body->get_path();
        brain.set_state(this, &scareRacoon);
    }
}






