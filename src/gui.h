#ifndef GUI_H
#define GUI_H

#include <Godot.hpp>
#include <Control.hpp>
#include <PopupMenu.hpp>
#include <stdlib.h>

namespace godot {

class Gui : public Control {
    GODOT_CLASS(Gui, Control)

private:
    //float time_passed;
    Vector3 velocity;
    Vector3 oldPos;

public:
    static void _register_methods();

    Gui();
    ~Gui();

    void _init(); // our initializer called by Godot

    void _ready();

    void _on_MenuButton_pressed();

    void _on_ExitButton_pressed();

    void _on_SFX_pressed();

    void _on_BackgroundSound_pressed();
};

}

#endif