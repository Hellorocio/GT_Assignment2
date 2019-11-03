#ifndef SQUIRREL_AI_H
#define SQUIRREL_AI_H

#include <Godot.hpp>
#include "base_ai.h"
#include <KinematicBody.hpp>
#include "waypoint.h"
#include <SceneTree.hpp>
#include <Node.hpp>
#include <Viewport.hpp>
#include <string>
#include "fsm.h"
#include <Area.hpp>

namespace godot {

	class FoundAcorn : public AbstractState {
	public:
		NodePath current_acorn = "";

		void start(Node* parent) override;
		void execute(Node* parent, float delta) override;
		void end(Node* parent) override;
	};

	class ScareRacoon : public AbstractState {
		public:
		NodePath racoon = "";

		void start(Node* parent) override;
		void execute(Node* parent, float delta) override;
		void end(Node* parent) override;
	};

	class SquirrelAI : public BaseAI {
		GODOT_SUBCLASS(SquirrelAI, BaseAI)
	public:
		WanderState wanderState;
		FoundAcorn foundAcorn;
		ScareRacoon scareRacoon;

		static void _register_methods();

		SquirrelAI();
		~SquirrelAI();

		void _init();
		void _ready();
		void _rotate_player();
		void _on_area_entered (Area* area);
		void _on_body_entered (Node* body);
	};
}

#endif