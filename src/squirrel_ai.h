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
			NodePath current_waypoint = "";
			NodePath previous_waypoint = "";
			NodePath temp_waypoint = "";

			void start(Node* parent) override;
			void execute(Node* parent) override;
			void end(Node* parent) override;
	};

	class SquirrelAI : public KinematicBody {
		GODOT_CLASS(SquirrelAI, KinematicBody)
	private:
        FSM brain;
		WanderState wanderState;

		Vector3 motion = Vector3(1, 0, 0);
		

	public:
		//Wander* wanderState = new Wander();
		float wander_speed = 2;
		float gravity = -40.0;

		static void _register_methods();

		SquirrelAI();
		~SquirrelAI();

		void _init();
		void _ready();
		void _process(float delta);
		void _physics_process(float delta);
		void _rotate_player();
		void _update_movement(Vector3 direction);
		NodePath _get_closest_node();
	};
}

#endif