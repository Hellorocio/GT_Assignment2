#ifndef TRAP_H
#define TRAP_H

#include <Godot.hpp>
#include <RigidBody.hpp>

namespace godot {

	class Trap : public RigidBody {
		GODOT_CLASS(Trap, RigidBody)

	private:
		

	public:
		static void _register_methods();

		Trap();
		~Trap();

		void _init();
		void _ready();
		void _process(float delta);

		void _on_body_entered(Node * body);
	};
}

#endif