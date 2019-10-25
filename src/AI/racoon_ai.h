#ifndef RACOON_AI_H
#define RACOON_AI_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include "fsm.h"

namespace godot {

	class RacoonAI : public KinematicBody {
		GODOT_CLASS(RacoonAI, KinematicBody)
	private:
        FSM brain;
	public:
		static void _register_methods();

		RacoonAI();
		~RacoonAI();

		void _init();
		void _ready();
		void _process(float delta);
	};
}

#endif