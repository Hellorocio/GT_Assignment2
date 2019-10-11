#include "player_handler.h"
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>
#include "basic_movement.h"

using namespace godot;

void PlayerHandler::_register_methods() {
	register_method("_ready", &PlayerHandler::_ready);
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

void PlayerHandler::_ready() {
	godot::BasicMovement* player = static_cast<godot::BasicMovement*>(PlayerScene->instance());
	add_child(player);
}

void PlayerHandler::_process(float delta) {

}