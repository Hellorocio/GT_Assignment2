#include "collectable.h"
#include "game_state.h"

using namespace godot;

void Collectable::_register_methods()
{
	register_method("init", &Collectable::init);
	register_method("_ready", &Collectable::_ready);
	register_method("_process", &Collectable::_process);
	register_method("_on_body_entered", &Collectable::_on_body_entered);
}

Collectable::Collectable()
{
}

Collectable::~Collectable()
{
}

void Collectable::_init()
{
}

void Collectable::init(Vector3 position)
{
	set_translation(position);
}

void Collectable::_ready()
{
	this->connect("body_entered", this, "_on_body_entered");
}

void Collectable::_on_body_entered(Node * body)
{
	BasicMovement * player = Object::cast_to<BasicMovement>(body);
	if (!is_killed && player != nullptr)
	{
		//increment player's acorn count
		GameState * state = Object::cast_to<GameState>(get_node("/root/Game/GameState"));
		state->collect_acorns();

		AudioStreamPlayer3D *a1 = Object::cast_to<AudioStreamPlayer3D>(body->get_node("AcornSound"));
		a1->play();

		//remove this node
		queue_free();
		is_killed = true;
	}
}

void Collectable::_process(float delta)
{
}

