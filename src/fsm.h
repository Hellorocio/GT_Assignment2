#ifndef FSM_H
#define FSM_H
#include <Node.hpp>

namespace godot {
    class AbstractState {

    public:
        virtual void start(Node* parent) {}
        virtual void execute(Node* parent) {}
        virtual void end(Node* parent) {}
    };
    

    class FSM { 

    private:
        AbstractState* state = nullptr;

    public:
        void update(Node* parent);
        void set_state(Node* parent, AbstractState* new_state);
    };
}

#endif