#ifndef TRAP_H
#define TRAP_H

#include <Godot.hpp>
#include <RigidBody.hpp>
#include <AudioStreamPlayer3D.hpp>

namespace godot {

	class Trap : public RigidBody {
		GODOT_CLASS(Trap, RigidBody)

	private:
		bool hitPlayer = false;

	public:
		static void _register_methods();

		Trap();
		~Trap();

		void _init();
		void _ready();
		void _process(float delta);

		void _on_body_entered(Node * body);
		void _on_body_exited(Node * body);
	};
}

#endif