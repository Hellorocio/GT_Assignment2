#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <Godot.hpp>
#include <Node.hpp>

namespace godot {

	class GameState : public Node {
		GODOT_CLASS(GameState, Node)

	private:

	public:
		int numCollected = 0;

		static void _register_methods();

		GameState();
		~GameState();

		void _init();
		void _ready();
		void _process(float delta);

		void collect();
	};
}

#endif