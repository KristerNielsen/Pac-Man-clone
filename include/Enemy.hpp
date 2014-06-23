#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <algorithm>
#include "Object.hpp"

class Game;
class Collision;

class Enemy : public Object {
        // Properties
        const unsigned int SIZE = 42;
        const unsigned short NO_FRAMES = 30;
        unsigned short targetX;
        unsigned short targetY;
        unsigned short dotCounter;
        std::string mode; // Chase, Scatter, Frightened, Dead, Idle
        unsigned short prevTileRow = -1;
        unsigned short prevTileCol = -1;

    public:
        // Methods
        Enemy(std::string _name);
        virtual ~Enemy();
        short getStartX();
        short getStartY();
        int getSize();
        short getNoFrames();
        std::vector<unsigned short> getTarget();
        void setTarget(short _row, short _col);
        std::string getMode();
        void setMode(std::string _mode);
        void findPath();
        float checkDistance(float x0, float y0, float x1, float y1);
        void setPrevTileRow(short _row);
        short getPrevTileRow();
        void setPrevTileCol(short _col);
        short getPrevTileCol();
};

#endif // ENEMY_H
