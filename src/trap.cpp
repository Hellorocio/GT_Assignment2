#include "trap.h"
#include "game_state.h"

using namespace godot;

void Trap::_register_methods() {
	register_method("_ready", &Trap::_ready);
	register_method("_process", &Trap::_process);
	register_method("_on_body_entered", &Trap::_on_body_entered);
	register_method("_on_body_exited", &Trap::_on_body_exited);
}

Trap::Trap() {
}

Trap::~Trap() {
}

void Trap::_init() {
}

void Trap::_ready() {
	this->connect("body_entered", this, "_on_body_entered");
	this->connect("body_exited", this, "_on_body_exited");
}

void Trap::_on_body_entered(Node * body) {
	BasicMovement * player = Object::cast_to<BasicMovement>(body);
	if (!hitPlayer && player != nullptr)	{
		//decrement player's acorn count
		GameState * state = Object::cast_to<GameState>(get_node("/root/Game/GameState"));
		state->remove_acorns();

		AudioStreamPlayer3D *a1 = Object::cast_to<AudioStreamPlayer3D>(body->get_node("TrapSound"));
		a1->play();

		//play particle effect
		Particles * acornParticles = (Particles *)body->get_node("Particles");
		if (acornParticles) {
			acornParticles->set_emitting(true);
		}

		hitPlayer = true;
	}
}

//Resets hitPlayer when the player exits
void Trap::_on_body_exited (Node * body) {
	BasicMovement * player = Object::cast_to<BasicMovement>(body);
	if (hitPlayer && player != nullptr) {
		hitPlayer = false;
	}
}

void Trap::_process(float delta) {
}

