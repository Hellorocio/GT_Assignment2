#include "collectable.h"
#include "game_state.h"

using namespace godot;

void Collectable::_register_methods()
{
	register_method("init", &Collectable::init);
	register_method("_ready", &Collectable::_ready);
	register_method("_process", &Collectable::_process);
	register_method("_on_body_entered", &Collectable::_on_body_entered);
	register_method("_on_collection", &Collectable::_on_collection, GODOT_METHOD_RPC_MODE_REMOTESYNC);
}

Collectable::Collectable()
{
}

Collectable::~Collectable()
{
}

void Collectable::_init()
{
}

void Collectable::init(Vector3 position)
{
	set_translation(position);
}

void Collectable::_ready()
{
	this->connect("body_entered", this, "_on_body_entered");
}

void Collectable::_on_body_entered(Node * body)
{
	BasicMovement * player = Object::cast_to<BasicMovement>(body);
	if (!is_killed && player != nullptr && (!get_tree()->has_network_peer() || player->is_network_master()))
	{
		// called by player's network master

		//increment player's acorn count
		int64_t player_id = body->get_name().to_int();

		GameState *state = Object::cast_to<GameState>(get_node("/root/Game/GameState"));
		if (get_tree()->has_network_peer())
    		state->rpc_id(1, "collect_acorn", player_id);
		else
			state->collect_acorn(player_id);

		if (get_tree()->has_network_peer())
    		rpc("_on_collection", player_id);
		else
			_on_collection(player_id);
	}
}

// called by everyone
void Collectable::_on_collection(int64_t id)
{
	AudioStreamPlayer3D *a1 = Object::cast_to<AudioStreamPlayer3D>(get_node("/root/Game/" + String::num_int64(id) + "/AcornSound"));
	a1->play();

	//remove this node
	queue_free();
	is_killed = true;
}

void Collectable::_process(float delta)
{
	auto network = get_node("/root/network");
    if (get_tree()->has_network_peer()) {
		if (get_tree()->is_network_server())
        	network->rpc_unreliable("update_acorn_position", get_name(), get_translation());
	}
    else
        network->call("update_acorn_position", get_name(), get_translation());
}

