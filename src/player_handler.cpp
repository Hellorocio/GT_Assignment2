#include "player_handler.h"
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>
#include "basic_movement.h"


using namespace godot;

void PlayerHandler::_register_methods() {
	register_method("_create_player", &PlayerHandler::_create_player);
	register_method("_create_acorns", &PlayerHandler::_create_acorns);
	register_method("_process", &PlayerHandler::_process);
	register_method("_ready", &PlayerHandler::_ready);
}

PlayerHandler::PlayerHandler() {
}

PlayerHandler::~PlayerHandler() {
}

void PlayerHandler::_ready()
{
	acorn_positions.resize(5);
	acorn_positions[0] = Vector3(11.9, 1.56, -4);
	acorn_positions[1] = Vector3(17.11, 0.87, -18.7);
	acorn_positions[2] = Vector3(3, 0.87, -18.7);
	acorn_positions[3] = Vector3(-21.4, 0.87, 4.67);
	acorn_positions[4] = Vector3(-21.4, 0.87, -19.8);
}

void PlayerHandler::_init() {
	ResourceLoader* resourceLoader = ResourceLoader::get_singleton();
    PlayerScene = resourceLoader->load("res://player.tscn");
	AcornScene = resourceLoader->load("res://acorn_collectable.tscn");

}

void PlayerHandler::_create_player() {
	godot::BasicMovement* player = static_cast<godot::BasicMovement*>(PlayerScene->instance());
	auto id = get_tree()->get_network_unique_id();
	player->set_name(String::num_int64(id));
    player->set_network_master(id);
	add_child(player);

	Dictionary self_data = Dictionary(get_node("/root/network")->get("self_data"));
    player->init(self_data["name"], self_data["position"], false);

	_create_acorns();
}

void PlayerHandler::_create_acorns() {
	for (int i = 0; i < acorn_positions.size(); i++) {
		godot::Collectable* acorn = static_cast<godot::Collectable*>(AcornScene->instance());
		add_child(acorn);

		acorn->init(acorn_positions[i]);
	}
}

void PlayerHandler::_process(float delta) {

}