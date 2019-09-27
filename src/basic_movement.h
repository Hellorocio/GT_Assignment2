#ifndef BASIC_MOVEMENT_H
#define BASIC_MOVEMENT_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include <Input.hpp>

namespace godot {

class BasicMovement : public KinematicBody {
    GODOT_CLASS(BasicMovement, KinematicBody)

private:
    //float time_passed;
    Vector3 motion;

public:
    static void _register_methods();

    BasicMovement();
    ~BasicMovement();

    void _init(); // our initializer called by Godot

    void _process(float delta);

    void UpdateMovementFromInput();
};

}

#endif