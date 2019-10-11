#include "player_handler.h"

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
}

void PlayerHandler::_ready() {
}

void PlayerHandler::_process(float delta) {

}