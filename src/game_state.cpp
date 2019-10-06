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
}

void GameState::_ready()
{
	numCollected = 0;
	Godot::print("ready acorn: ");
	std::string s1 = std::to_string(numCollected);
	String s2 = s1.c_str();
	Godot::print("" + s2);
}

void GameState::collect_acorns()
{
	Godot::print("collect acorn 1: ");
	std::string s1 = std::to_string(numCollected);
	String s2 = s1.c_str();
	Godot::print("" + s2);
	numCollected++;

	Godot::print("collect acorn 2: ");
	s1 = std::to_string(numCollected);
	s2 = s1.c_str();
	Godot::print("" + s2);

	Label* label = (Label*) get_parent()->get_node("GUI/HSplitContainer/NinePatchRect/Label");
	if (label) {
		label->set_text(s2);
	}

	if (numCollected >= 5) {
		Godot::print("you won");
		PopupMenu* menu = (PopupMenu*) get_parent()->get_node("WinMenu");
		if (menu) {
			menu->show();
			get_tree()->set_pause(true);
		}
	}
}

void GameState::remove_acorns()
{
	if (numCollected > 0)
	{
		numCollected--;
	}
	Godot::print("lose acorn: ");
	Godot::print(std::to_string(numCollected).c_str());

	//update UI
	Label* label = (Label*) get_parent()->get_node("GUI/HSplitContainer/NinePatchRect/Label");
	if (label) {
		label->set_text(std::to_string(numCollected).c_str());
	}

	//play particle effect
	Particles * acornParticles = (Particles *)get_node("/root/Spatial/Player/Particles");
	if (acornParticles)
	{
		acornParticles->set_emitting(true);
	}
}

void GameState::_process(float delta)
{
}

