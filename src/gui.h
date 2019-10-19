#ifndef GUI_H
#define GUI_H

#include <Godot.hpp>
#include <Control.hpp>
#include <stdlib.h>
#include <AudioStreamPlayer3D.hpp>
#include <SceneTree.hpp>
#include <Range.hpp>
#include "basic_movement.h"
#include "network.h"
#include <LineEdit.hpp>
#include <Label.hpp>
#include <Timer.hpp>
#include <list>

namespace godot {

class Gui : public Control {
    GODOT_CLASS(Gui, Control)

private:
    //float time_passed;
    Vector3 velocity;
    Vector3 oldPos;
    bool soundEffect;
    bool backgroundSound;
    int time_left = 0;            

public:
    static void _register_methods();

    Gui();
    ~Gui();

    void _init(); // our initializer called by Godot

    void _ready();

    void _process();

    void _on_MenuButton_pressed();

    void _on_ExitButton_pressed();

    void _on_SFX_pressed();

    void _on_BackgroundSound_pressed();

    void _on_RotateStrafe_pressed();

    void _on_PlayAgain_pressed();

    void _on_QuitButton_pressed();

    void _on_PlayMain_pressed();

    void _on_CreateMain_pressed ();
    
    void _on_JoinMain_pressed ();

    void _on_LobbyPlay_pressed();

    void _on_JoinIPMain_pressed();

    void _WinMenu_show(bool win);

    void _on_VolumeSlider_changed(float value);

    void _update_acorn_count (String count);

    void _update_timer();

    void _set_time_label();

    void _on_timeout();
};

}

#endif
