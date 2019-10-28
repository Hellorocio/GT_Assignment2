#ifndef SQUIRREL_AI_H
#define SQUIRREL_AI_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include "waypoint.h"
#include <SceneTree.hpp>
#include <Node.hpp>
#include <Viewport.hpp>
#include <string>
#include "fsm.h"

namespace godot {

	class WanderState : public AbstractState {
		public:
			Waypoint* current_waypoint = nullptr;
			Waypoint* previous_waypoint = nullptr;
			Node* squirrelAI= nullptr;

			WanderState(Node* squirrel);
			void start() override;
			void execute() override;
			void end() override;
	};

	class SquirrelAI : public KinematicBody {
		GODOT_CLASS(SquirrelAI, KinematicBody)
	private:
        FSM brain;
		WanderState* wanderState = nullptr;

		Vector3 motion = Vector3(1, 0, 0);
		

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
		void _update_movement(Vector3 direction);
		Waypoint* _get_closest_node();
	};
}

#endif