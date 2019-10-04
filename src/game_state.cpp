#include "game_state.h"

using namespace godot;

void GameState::_register_methods()
{
	register_method("_ready", &GameState::_ready);
	register_method("_process", &GameState::_process);
	register_method("collect_acorns", &GameState::collect_acorns);
	register_method("remove_acorns", &GameState::remove_acorns);
}

GameState::GameState()
{
}

GameState::~GameState()
{
}

void GameState::_init()
{
	numCollected = 0;
}

void GameState::_ready()
{
}

void GameState::collect_acorns()
{
	numCollected++;

	Godot::print("collect acorn: ");
	//Godot::print(std::to_string(numCollected).c_str());
	std::string s1 = std::to_string(numCollected);
	String s2 = s1.c_str();
	Godot::print("" + s2);
}

void GameState::remove_acorns()
{
	if (numCollected > 0)
	{
		numCollected--;
	}
	Godot::print("lose acorn: ");
	Godot::print(std::to_string(numCollected).c_str());
}

void GameState::_process(float delta)
{
}

