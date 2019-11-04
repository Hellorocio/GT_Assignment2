#ifndef BASE_AI_H
#define BASE_AI_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include "waypoint.h"
#include <SceneTree.hpp>
#include <Node.hpp>
#include <Viewport.hpp>
#include <string>
#include "fsm.h"
#include <Area.hpp>

namespace godot {

	class WanderState : public AbstractState {
		bool just_awoke = false;
	public:
        Array path = Array{};

		void start(Node* parent) override;
		void execute(Node* parent, float delta) override;
		void end(Node* parent) override;
	};

	class BaseAI : public KinematicBody {
		GODOT_CLASS(BaseAI, KinematicBody)
	private:
		Vector3 motion = Vector3(1, 0, 0);
		Vector3 prev_translation = Vector3(0, 0, 0);

		float total_time_stuck = 0.0f;

	public:
		FSM brain;
		WanderState wanderState;
		
		float movement_speed = 6;
		float gravity = -40.0;
		bool was_stopped = false;

		static void _register_methods();

        void sleep(float time_in_seconds, AbstractState* next_state);
        
        virtual void _process(float delta);
        virtual void _physics_process(float delta);
		void _update_movement(Vector3 direction, float delta = 1.0f);
		void _turn_to_face(Vector3 target);

        Vector3 get_movement_vector_to_target(Vector3 target, bool& finished);

		NodePath get_closest_node_to_point(Vector3 pos, float lowest_y_delta = -1.0f/0.0f, float highest_y_delta = 1.0f/0.0f);
        NodePath get_farthest_node_to_point(Vector3 pos);

        PoolVector3Array get_shortest_path(Vector3 from, Vector3 to);
	private:
        PoolVector3Array calculate_shortest_path(NodePath from, NodePath to);
	};
}

#endif