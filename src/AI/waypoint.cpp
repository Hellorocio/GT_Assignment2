#include "waypoint.h"

using namespace godot;

void Waypoint::_register_methods() {
	register_method("_ready", &Waypoint::_ready);

    register_property<Waypoint, PoolStringArray>("adjacent", &Waypoint::adjacent, PoolStringArray{});
}

Waypoint::Waypoint() {
}

Waypoint::~Waypoint() {
}

void Waypoint::_init() {
    Godot::print("hi again");
}

void Waypoint::_ready() {
}

