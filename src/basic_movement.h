#ifndef BASIC_MOVEMENT_H
#define BASIC_MOVEMENT_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include <Input.hpp>
#include <string>
#include <InputEvent.hpp>

namespace godot {

class BasicMovement : public KinematicBody {
    GODOT_CLASS(BasicMovement, KinematicBody)

private:
    // movement
    Vector3 motion;

    Vector3 forward;
    Vector3 right;
    float movement_speed = 8;
    float gravity = -10.0;
    float falling_speed;
    int jump_frame;

    // camera properties
    Vector2 mouse_delta;
    float yaw = 0; // horizontal
    float pitch = 0; // vertical

    float horiz_camera_sensitivity = 6;
    float vert_camera_sensitivity = 6;
    bool invert_x_axis = false;
    bool invert_y_axis = false;

public:
    static void _register_methods();

    BasicMovement();
    ~BasicMovement();

    void _init();

    void _input(InputEvent *event);

    void _process(float delta);

    void _physics_process(float delta);

    void update_camera(float delta);

    void update_movement_from_input(float delta);
};

}

#endif
