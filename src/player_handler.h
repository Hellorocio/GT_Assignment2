#ifndef PLAYER_HANDLER_H
#define PLAYER_HANDLER_H

#include <Godot.hpp>
#include <Node.hpp>

namespace godot {

	class PlayerHandler : public Node {
		GODOT_CLASS(PlayerHandler, Node)

	private:

	public:
		static void _register_methods();

		PlayerHandler();
		~PlayerHandler();

		void _init();
		void _ready();
		void _process(float delta);
	};
}

#endif