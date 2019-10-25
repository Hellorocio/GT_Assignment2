#ifndef SQUIRREL_AI_H
#define SQUIRREL_AI_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include "fsm.h"

namespace godot {

	class SquirrelAI : public KinematicBody {
		GODOT_CLASS(SquirrelAI, KinematicBody)
	private:
        FSM brain;
	public:
		static void _register_methods();

		SquirrelAI();
		~SquirrelAI();

		void _init();
		void _ready();
		void _process(float delta);
	};
}

#endif