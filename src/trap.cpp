#include "trap.h"
#include "game_state.h"

using namespace godot;

void Trap::_register_methods()
{
	register_method("_ready", &Trap::_ready);
	register_method("_process", &Trap::_process);
	register_method("_on_body_entered", &Trap::_on_body_entered);
}

Trap::Trap()
{
}

Trap::~Trap()
{
}

void Trap::_init()
{
}

void Trap::_ready()
{
	this->connect("body_entered", this, "_on_body_entered");
}

void Trap::_on_body_entered(Node * body)
{
	if (body->get_name() == "Player")
	{
		//decrement player's acorn count
		GameState * state = (GameState *)get_node("/root/Spatial/GameState");
		state->remove_acorns();
	}
	
}

void Trap::_process(float delta)
{
}

