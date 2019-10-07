#ifndef TRAP_H
#define TRAP_H

#include <Godot.hpp>
#include <Area.hpp>
#include <AudioStreamPlayer3D.hpp>

namespace godot {

	class Trap : public Area {
		GODOT_CLASS(Trap, Area)

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