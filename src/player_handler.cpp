#include "player_handler.h"
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>
#include "basic_movement.h"

using namespace godot;

void PlayerHandler::_register_methods() {
	register_method("_create_player", &PlayerHandler::_create_player);
	register_method("_process", &PlayerHandler::_process);
}

PlayerHandler::PlayerHandler() {
}

PlayerHandler::~PlayerHandler() {
}

void PlayerHandler::_init() {
	ResourceLoader* resourceLoader = ResourceLoader::get_singleton();
    PlayerScene = resourceLoader->load("res://player.tscn");
}

void PlayerHandler::_create_player() {
	godot::BasicMovement* player = static_cast<godot::BasicMovement*>(PlayerScene->instance());
	auto id = get_tree()->get_network_unique_id();
	player->set_name(String::num_int64(id));
    player->set_network_master(id);
	add_child(player);

	Dictionary self_data = Dictionary(get_node("/root/network")->get("self_data"));
    player->init(self_data["name"], self_data["position"], false);
}

void PlayerHandler::_process(float delta) {

}