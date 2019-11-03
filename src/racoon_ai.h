#ifndef RACOON_AI_H
#define RACOON_AI_H

#include <Godot.hpp>
#include "base_ai.h"
#include <KinematicBody.hpp>
#include "squirrel_ai.h"
#include "fsm.h"
#include <Node.hpp>

namespace godot {
	class ChaseState : public AbstractState {
	public:
		NodePath current_target = "";
		NodePath current_target_rotation = "";

		void start(Node* parent) override;
		void execute(Node* parent, float delta) override;
		void end(Node* parent) override;
	};

	class RunAwayState : public AbstractState {
		public:

		void start(Node* parent) override;
		void execute(Node* parent, float delta) override;
		void end(Node* parent) override;
	};

	class RacoonAI : public BaseAI {
		GODOT_SUBCLASS(RacoonAI, BaseAI)
	public:
		WanderState wanderState;
		ChaseState chaseState;
		RunAwayState runAwayState;

		static void _register_methods();

		RacoonAI();
		~RacoonAI();

		void _init();
		void _ready();
		void _on_body_entered (Node* body);
	};
}

#endif