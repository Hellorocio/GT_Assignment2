#ifndef FSM_H
#define FSM_H
#include <Node.hpp>

namespace godot {
    class AbstractState {

    public:
        virtual void start(Node* parent) {}
        virtual void execute(Node* parent, float delta) {}
        virtual void end(Node* parent) {}
    };
    

    class FSM { 

    public:
        AbstractState* state = nullptr;
        void update(Node* parent, float delta);
        void set_state(Node* parent, AbstractState* new_state);
    };
}

#endif