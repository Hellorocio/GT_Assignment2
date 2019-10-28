#ifndef SQUIRREL_AI_H
#define SQUIRREL_AI_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include "fsm.h"
#include "waypoint.h"
#include <SceneTree.hpp>
#include <Node.hpp>
#include <Viewport.hpp>
#include <string>

namespace godot {

	class WanderState : public AbstractState {
	public:
		void start() override;
		void execute() override;
		void end() override;
	};

	class SquirrelAI : public KinematicBody {
		GODOT_CLASS(SquirrelAI, KinematicBody)
	private:
        FSM brain;
		WanderState wanderState;

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
		Waypoint* _get_closest_node();
	};

	
}

#endif