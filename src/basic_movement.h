#ifndef BASIC_MOVEMENT_H
#define BASIC_MOVEMENT_H

#include <Godot.hpp>
#include <KinematicBody.hpp>
#include <Input.hpp>
#include <string>
#include <InputEvent.hpp>
#include <AudioStreamPlayer3D.hpp>

namespace godot {

enum PlayerState { GROUNDED, JUMP, SLIDE, FALL, GLIDING, LEDGE_HANGING };

class BasicMovement : public KinematicBody {
    GODOT_CLASS(BasicMovement, KinematicBody)

private:
    // physics
    float falling_speed = -100.0; //This is what we are using to handle air resistence, the higher the number the faster the player falls
    float max_falling_speed = -100.0;
    float max_glide_fall_speed = -3.0;
    float walkable_angle = 0.685398; 

    // movement
    Vector3 motion;
    Vector3 acceleration;
    PlayerState state = GROUNDED;

    Vector3 forward;
    Vector3 right;
    float movement_speed = 8;
    float gravity = -40.0;

    float walk_speed = 8.0;
    float sprint_speed = 32.0;
    float glide_speed = 16.0;
    
    float ad_rotate_speed = 3.14f;

    bool ad_rotate = false; //if true, a and d keys rotate the player. Otherwise a and d strafe
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

    void toggle_AD_rotate ();
};

}

#endif
