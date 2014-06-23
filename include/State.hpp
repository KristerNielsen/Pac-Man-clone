#ifndef STATE_H
#define STATE_H

#include "Object.hpp"

class State {
    public:
        // Methods
        static short animate(Object* _obj);
        static short death(Object* _obj);
};

#endif // STATE_H
