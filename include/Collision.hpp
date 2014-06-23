#ifndef COLLISION_H
#define COLLISION_H

#include <iostream>

class Object;
class Game;

class Collision {
        //Methods
        static void setSpeedToZeroOnCollision(Object* _obj);
    public:
        // Methods
        static bool check(Object* _obj);
        static bool checkInInputDir(Object* _obj, int _dir);
};

#endif // COLLISION_H
