#include "racoon_ai.h"

using namespace godot;

void ChaseState::start(Node* parent) {

}

void ChaseState::execute(Node* parent, float delta)
{

}

void ChaseState::end(Node* parent) {

}

void RunAwayState::start(Node* parent) {

}

void RunAwayState::execute(Node* parent, float delta) {

}

void RunAwayState::end(Node* parent) {
    
}

void RacoonAI::_register_methods() {
	register_method("_ready", &RacoonAI::_ready);
    register_method("_process", &RacoonAI::_process);
    register_method("_on_body_entered", &RacoonAI::_on_body_entered);
}

RacoonAI::RacoonAI() {
}

RacoonAI::~RacoonAI() {
}

void RacoonAI::_init() {
}

void RacoonAI::_ready() {
   // brain.set_state(this, &wanderState);

    //get_node("RaccoonVision")->connect("body_entered", this, "_on_body_entered");
}

void RacoonAI::_on_body_entered (Node* body) {
    Godot::print(body->get_name());
}

void RacoonAI::_process(float delta) {

}



