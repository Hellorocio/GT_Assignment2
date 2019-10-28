#ifndef FSM_H
#define FSM_H

namespace godot {
    class AbstractState {

    public:
        virtual void start() {}
        virtual void execute() {}
        virtual void end() {}
    };
    

    class FSM { 

    private:
        AbstractState* state = nullptr;

    public:
        void update();
        void set_state(AbstractState* new_state);
    };

    
}

#endif