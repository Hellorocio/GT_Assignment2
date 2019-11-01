#ifndef RACOON_AI_H
#define RACOON_AI_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include "squirrel_ai.h"
#include "fsm.h"

namespace godot {
	class ChaseState : public AbstractState {
	public:

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

	class RacoonAI : public KinematicBody {
		GODOT_CLASS(RacoonAI, KinematicBody)
	private:
        FSM brain;
		
		ChaseState chaseState;
		RunAwayState runAwayState;

		friend class ChaseState;
		friend class RunAwayState;
	public:
		static void _register_methods();

		RacoonAI();
		~RacoonAI();

		void _init();
		void _ready();
		void _process(float delta);
		void _on_body_entered (Node* body);
	};
}

#endif