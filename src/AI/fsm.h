#ifndef FSM_H
#define FSM_H

class AbstractState {
private:

public:

    AbstractState();
    ~AbstractState();

    virtual void start() = 0;
    virtual void excecute() = 0;
    virtual void end() = 0;
};

class FSM { 

private:
    AbstractState* state;

public:

    FSM();
    ~FSM();

    void update();
    void set_state();
};

#endif