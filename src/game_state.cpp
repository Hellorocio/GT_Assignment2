#include "game_state.h"

using namespace godot;

void GameState::_register_methods() {
	register_method("_ready", &GameState::_ready);
	register_method("collect_acorns", &GameState::collect_acorns);
	register_method("remove_acorns", &GameState::remove_acorns);
	register_method("_process", &GameState::_process);
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
	numCollected = 0;
	Godot::print("ready acorn: ");
	Godot::print(String::num_int64(numCollected));
}

void GameState::collect_acorns() {
	Godot::print("collect acorn 1: ");
	String gs1 = String::num_int64(numCollected);
	Godot::print(gs1);
	numCollected++;

	Godot::print("collect acorn 2: ");
	String gs2 = String::num_int64(numCollected);
	Godot::print(gs2);

	Label* label = (Label*) get_parent()->get_node("GUI/HSplitContainer/NinePatchRect/Label");
	if (label) {
		label->set_text(gs2);
	}

	if (numCollected >= 20) {
		//Godot::print("you won");
		Gui* gui = (Gui*) get_parent()->get_node("GUI");
		if (gui) {
			gui->_WinMenu_show();
		}
	}
}

void GameState::remove_acorns() {
	if (numCollected > 0) {
		numCollected--;
	}

	Godot::print("lose acorn: ");
	String gs1 = String::num_int64(numCollected);
	Godot::print(gs1);

	//update UI
	Label* label = (Label*) get_parent()->get_node("GUI/HSplitContainer/NinePatchRect/Label");
	if (label) {
		label->set_text(gs1);
	}

	//play particle effect
	Particles * acornParticles = (Particles *)get_node("/root/Spatial/Player/Particles");
	if (acornParticles) {
		acornParticles->set_emitting(true);
	}
}

