#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include <Godot.hpp>
#include <RigidBody.hpp>
#include <AudioStreamPlayer3D.hpp>

namespace godot {

	class Collectable : public RigidBody {
		GODOT_CLASS(Collectable, RigidBody)

	private:
		

	public:
		static void _register_methods();

		Collectable();
		~Collectable();

		void _init();
		void _ready();
		void _process(float delta);

		void _on_body_entered(Node * body);
	};
}

#endif