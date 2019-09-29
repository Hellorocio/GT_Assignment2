#include "collectable.h"

using namespace godot;

void Collectable::_register_methods()
{
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

void Collectable::_ready()
{
	this->connect("body_entered", this, "_on_body_entered");
}

void Collectable::_on_body_entered(Node * body)
{
	if (body->get_name() == "Player")
	{
		Godot::print("Collided with player!!");

		//TODO: increment player's acorn count

		//remove this node
		queue_free();
	}
	
}

void Collectable::_process(float delta)
{
}

