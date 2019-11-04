#include "trap.h"
#include "game_state.h"

using namespace godot;

void Trap::_register_methods() {
	register_method("_ready", &Trap::_ready);
	register_method("_process", &Trap::_process);
	register_method("_on_body_entered", &Trap::_on_body_entered);
	register_method("_on_player_hit", &Trap::_on_player_hit, GODOT_METHOD_RPC_MODE_REMOTESYNC);
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

void Trap::_on_body_entered(Node* body) {
	BasicMovement* player = Object::cast_to<BasicMovement>(body);
	if (!hitPlayer && player != nullptr && (!get_tree()->has_network_peer() || player->is_network_master())) {
		//decrement player's acorn count
		int64_t player_id = body->get_name().to_int();

		GameState * state = Object::cast_to<GameState>(get_node("/root/Game/GameState"));
		if (get_tree()->has_network_peer())
    		state->rpc_id(1, "remove_acorn", player_id);
		else
			state->remove_acorn(player_id);

		if (get_tree()->has_network_peer())
    		rpc("_on_player_hit", player_id);
		else
			_on_player_hit(player_id);
	}
}

void Trap::_on_player_hit(int64_t id) {
	AudioStreamPlayer3D *a1 = Object::cast_to<AudioStreamPlayer3D>(get_node("/root/Game/" + String::num_int64(id) + "/TrapSound"));
	a1->play();

	//play particle effect
	Particles * acornParticles = Object::cast_to<Particles>(get_node("/root/Game/" + String::num_int64(id) + "/Particles"));
	if (acornParticles) {
		acornParticles->set_emitting(true);
	}
}

void Trap::_on_body_exited (Node * body) {
}

void Trap::_process(float delta) {
}

