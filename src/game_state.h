#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <Godot.hpp>
#include <Node.hpp>
#include <Label.hpp>
#include <SceneTree.hpp>
#include <string>
#include <Particles.hpp>
#include <gui.h>

namespace godot {

	class GameState : public Node {
		GODOT_CLASS(GameState, Node)

	private:

	public:
		int num_acorns = 25;
		Vector2 max_acorn_bounds = Vector2{10, 10};
		Vector2 min_acorn_bounds = Vector2{-10, 10};
		int numCollected = 0;

		static void _register_methods();

		GameState();
		~GameState();

		void _init();
		void _ready();
		void _process(float delta);

		void collect_acorns(int64_t id);
		void remove_acorns();
	};
}

#endif