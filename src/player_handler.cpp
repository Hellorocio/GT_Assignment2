#include "player_handler.h"
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>
#include "basic_movement.h"


using namespace godot;

void PlayerHandler::_register_methods() {
	register_method("_create_player", &PlayerHandler::_create_player);
	register_method("_process", &PlayerHandler::_process);
	register_method("_ready", &PlayerHandler::_ready);
}

PlayerHandler::PlayerHandler() {
}

PlayerHandler::~PlayerHandler() {
}

void PlayerHandler::_ready()
{
	//OS* os = OS::get_singleton();
	//os->set_window_size(os->get_screen_size());
}

void PlayerHandler::_init() {
	ResourceLoader* resourceLoader = ResourceLoader::get_singleton();
    PlayerScene = resourceLoader->load("res://player.tscn");
}

void PlayerHandler::_create_player() {
	BasicMovement* player = Object::cast_to<BasicMovement>(PlayerScene->instance());
	player->set_name("1");

	if (!get_tree()->has_network_peer())
	{
		//not in a network (used only for single player mode)
	}
	else if (get_tree()->has_network_peer() || get_tree()->is_network_server())
	{
		auto id = get_tree()->get_network_unique_id();
		player->set_name(String::num_int64(id));
		player->set_network_master(id);
	}
	add_child(player);

	Dictionary self_data = (Dictionary) (get_node("/root/network")->get("self_data"));
    player->init(self_data["name"], self_data["position"], false);
}


void PlayerHandler::_process(float delta) {

}