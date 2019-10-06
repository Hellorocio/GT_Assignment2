#ifndef BASIC_MOVEMENT_H
#define BASIC_MOVEMENT_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include <Input.hpp>
#include <string>
#include <InputEvent.hpp>

namespace godot {

enum PlayerState { GROUNDED, JUMP, SLIDE, FALL, GLIDING, LEDGE_HANGING };

class BasicMovement : public KinematicBody {
    GODOT_CLASS(BasicMovement, KinematicBody)

private:
    // physics
    float falling_speed_max = -100.0;

    // movement
    Vector3 motion;
    Vector3 acceleration;
    PlayerState state = GROUNDED;

    Vector3 forward;
    Vector3 right;
    float movement_speed = 8;
    float gravity = -40.0;
    
    bool ad_rotate = false; //if true, a and d keys rotate the player. Otherwise a and d strafe
    float ad_rotate_speed = 3.14f;

    bool sprinting = false;

    float ledge_stop_test_distance = 0.65f;
    Vector3 last_ledge_stop_horiz_movement;

    float ledge_grab_distance = 2.0f;
    bool can_ledge_hang = true;
    float ledge_grab_cooldown = 1.0f;

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

    void _ready();

    void _input(InputEvent *event);

    void _process(float delta);

    void _physics_process(float delta);

    void update_camera(float delta);

    void rotate_player();

    void update_camera_target();

    void update_movement(float delta);
};

}

#endif
