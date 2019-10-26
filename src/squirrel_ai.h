#ifndef SQUIRREL_AI_H
#define SQUIRREL_AI_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include "fsm.h"

namespace godot {

	class Wander : public AbstractState {
	public:
		Wander();
		~Wander();

		void start();
		void execute();
		void end();
	};

	class SquirrelAI : public KinematicBody {
		GODOT_CLASS(SquirrelAI, KinematicBody)
	private:
        //FSM* brain = new FSM();

	public:
		//Wander* wanderState = new Wander();

		static void _register_methods();

		SquirrelAI();
		~SquirrelAI();

		void _init();
		void _ready();
		void _process(float delta);
		void _physics_process(float delta);
		void _rotate_player();
		void _update_movement(float delta);
	};

	
}

#endif