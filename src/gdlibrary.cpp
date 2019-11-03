#include "basic_movement.h"
#include "collectable.h"
#include "trap.h"
#include "game_state.h"
#include "gui.h"
#include "player_handler.h"
#include "network.h"
#include "squirrel_ai.h"
#include "racoon_ai.h"
#include "waypoint.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);

    godot::register_class<godot::BasicMovement>();
	godot::register_class<godot::Collectable>();
	godot::register_class<godot::Trap>();
	godot::register_class<godot::GameState>();
    godot::register_class<godot::Gui>();
    godot::register_class<godot::PlayerHandler>();
    godot::register_class<godot::Network>();
    godot::register_class<godot::BaseAI>();
    godot::register_class<godot::SquirrelAI>();
    godot::register_class<godot::RacoonAI>();
    godot::register_class<godot::Waypoint>();


}