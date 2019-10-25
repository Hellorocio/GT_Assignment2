#ifndef FSM_H
#define FSM_H

namespace godot {
    class AbstractState {

    public:
        AbstractState();
        virtual ~AbstractState();

        virtual void start() = 0;
        virtual void execute() = 0;
        virtual void end() = 0;
    };

    class FSM { 

    private:
        AbstractState* state;

    public:

        FSM();
        ~FSM();

        void update();
        void set_state(AbstractState* new_state);
    };
}

#endif