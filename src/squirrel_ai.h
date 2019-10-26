#ifndef SQUIRREL_AI_H
#define SQUIRREL_AI_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
//#include "fsm.h"
#include "waypoint.h"
#include <SceneTree.hpp>
#include <Node.hpp>
#include <Viewport.hpp>
#include <string>

namespace godot {


	enum AIState { STOP = 0, WANDER = 1 };

	/*
	class Wander : public AbstractState {
	public:
		Wander();
		~Wander();

		void start();
		void execute();
		void end();
	};
	*/

	class SquirrelAI : public KinematicBody {
		GODOT_CLASS(SquirrelAI, KinematicBody)
	private:
        //FSM* brain = new FSM();
		AIState state = STOP;
		Vector3 motion = Vector3(1, 0, 0);

		Waypoint* current_waypoint;
		Waypoint* previous_waypoint;
		

	public:
		//Wander* wanderState = new Wander();
		float wander_speed = 2;

		static void _register_methods();

		SquirrelAI();
		~SquirrelAI();

		void _init();
		void _ready();
		void _process(float delta);
		void _physics_process(float delta);
		void _rotate_player();
		void _update_movement(float delta);
		void _change_state(int s);
		Waypoint* _get_closest_node();
	};

	
}

#endif