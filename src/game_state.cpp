#include "game_state.h"

using namespace godot;

void GameState::_register_methods() {
	register_method("_ready", &GameState::_ready);
	register_method("collect_acorn", &GameState::collect_acorn, GODOT_METHOD_RPC_MODE_REMOTESYNC);
	register_method("on_collect_acorn", &GameState::on_collect_acorn, GODOT_METHOD_RPC_MODE_REMOTESYNC);
	register_method("remove_acorn", &GameState::remove_acorn, GODOT_METHOD_RPC_MODE_REMOTESYNC);
	register_method("_process", &GameState::_process);

	register_property<GameState, int>("num_acorns", &GameState::num_acorns, 25);
	register_property<GameState, Vector2>("max_acorn_bounds", &GameState::max_acorn_bounds, Vector2{10, 10});
	register_property<GameState, Vector2>("min_acorn_bounds", &GameState::min_acorn_bounds, Vector2{-10, -10});
}

GameState::GameState() {
}

GameState::~GameState() {
}

void GameState::_init() {
}

void GameState::_process(float delta) {
}

void GameState::_ready() {
	num_collected = 0;
}

// only called by server
void GameState::collect_acorn(int64_t id) {
	++num_collected;
	Dictionary self_data = (Dictionary) (get_node("/root/network")->get("self_data"));
	self_data["acorns_collected"] = num_collected;

	//Godot::print("collect acorn: " + String::num_int64(get_tree()->get_network_unique_id()));
	if (get_tree()->has_network_peer()) {
		rpc("on_collect_acorn", id, num_collected);
	}
	else
		on_collect_acorn(id, num_collected);
}

// called for everyone
void GameState::on_collect_acorn(int64_t id, int num) {
	//Godot::print("on collect acorn: " + String::num_int64(get_tree()->get_network_unique_id()));
	Dictionary self_data = (Dictionary) get_node("/root/network")->get("self_data");
	num_collected = num;
	self_data["acorns_collected"] = num;

	String gs2 = String::num_int64(num);
	Gui* gui = Object::cast_to<Gui>(get_parent()->get_node("GUI"));
	gui->_update_acorn_count(gs2);

	// win condition
	if (num >= 20) {
		gui->_WinMenu_show(true);
	}
}

// only called by server
void GameState::remove_acorn(int64_t id) {
	Dictionary self_data = (Dictionary) (get_node("/root/network")->get("self_data"));
	if (num_collected > 0) {
		--num_collected;
	}
	self_data["acorns_collected"] = num_collected;

	if (get_tree()->has_network_peer()) {
		rpc("on_collect_acorn", id, num_collected);
	}
	else
		on_collect_acorn(id, num_collected);
}