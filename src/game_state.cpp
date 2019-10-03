#include "game_state.h"

using namespace godot;

void GameState::_register_methods()
{
	register_method("_ready", &GameState::_ready);
	register_method("_process", &GameState::_process);
	register_method("collect", &GameState::collect);
}

GameState::GameState()
{
}

GameState::~GameState()
{
}

void GameState::_init()
{
	Godot::print("game state init");
}

void GameState::_ready()
{
}

void GameState::collect()
{
	numCollected++;
}

void GameState::_process(float delta)
{
}

