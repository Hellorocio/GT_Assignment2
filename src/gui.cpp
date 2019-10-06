#include "gui.h"

using namespace godot;

void Gui::_register_methods() {
    register_method("_on_MenuButton_pressed", &Gui::_on_MenuButton_pressed);
	register_method("_on_ExitButton_pressed", &Gui::_on_ExitButton_pressed);
	register_method("_on_SFX_pressed", &Gui::_on_SFX_pressed);
	register_method("_on_BackgroundSound_pressed", &Gui::_on_BackgroundSound_pressed);
    register_method("_ready", &Gui::_ready);
}

Gui::Gui() {

}
Gui::~Gui() {

}

void Gui::_init() {
    
}

void Gui::_ready() {
    //Node* button = get_node("/root/GUI/HSplitContainer/MenuButton");
    Node* button = get_node("HSplitContainer/MenuButton");
	if (button) {
		//Godot::print("connecting2");
		button->connect("pressed", this, "_on_MenuButton_pressed");
	}
	Node* exitButton = get_parent()->get_node("PopupMenu/ExitButton");
	if (exitButton) {
		exitButton->connect("pressed", this, "_on_ExitButton_pressed");
	}
	Node* SFXButton = get_parent()->get_node("PopupMenu/HBoxContainer/SFX");
	if (SFXButton) {
		SFXButton->connect("pressed", this, "_on_SFX_pressed");
	}
	Node* BackgroundButton = get_parent()->get_node("PopupMenu/HBoxContainer/BackgroundSound");
	if (BackgroundButton) {
		BackgroundButton->connect("pressed", this, "_on_BackgroundSound_pressed");
	}
}

void Gui::_on_MenuButton_pressed() {
    Godot::print("in on button pressed");
	PopupMenu* menu = (PopupMenu*) get_parent()->get_node("PopupMenu");
	if (menu) {
		Godot::print("menu != null");
		((PopupMenu*) menu)->show();
	}
	else
		Godot::print("menu == null");
}

void Gui::_on_ExitButton_pressed() {
	PopupMenu* menu = (PopupMenu*) get_parent()->get_node("PopupMenu");
	if (menu) {
		Godot::print("menu != null");
		((PopupMenu*) menu)->hide();
	}
}

void Gui::_on_SFX_pressed() {

}

void Gui::_on_BackgroundSound_pressed() {
	
}