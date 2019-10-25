#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <Godot.hpp>
#include <Spatial.hpp>

namespace godot {

	class Waypoint : public Spatial {
		GODOT_CLASS(Waypoint, Spatial)

        
        
	public:
        PoolStringArray adjacent;

		static void _register_methods();

		Waypoint();
		~Waypoint();

		void _init();
		void _ready();
	};
}

#endif