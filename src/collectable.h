#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include <Godot.hpp>
#include <RigidBody.hpp>
#include <AudioStreamPlayer3D.hpp>
#include "squirrel_ai.h"

namespace godot {

	class Collectable : public RigidBody {
		GODOT_CLASS(Collectable, RigidBody)

	private:
		bool is_killed = false;

	public:
		static void _register_methods();

		Collectable();
		~Collectable();

		void _init();
		void _ready();
		void _process(float delta);
		void init(Vector3 start_position);
		void _on_body_entered(Node * body);
		void _on_collection(String name);
	};
}

#endif