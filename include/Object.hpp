#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>
#include <vector>
#include <allegro5/allegro.h>

class Game;

class Object {
        bool outOfBounds;
    protected:
        // Properties
        std::string name;
        unsigned short startX; // Object start position
        unsigned short startY;
        float x; // Object position at its center point
        float y;
        int currTileCol; // Current tile the object is occupying
        int currTileRow;
        float speed; // Object speed
        int dir; // Direction the object is facing 0 = up, 1 = right, 2 = down, 3 = left
        ALLEGRO_BITMAP* img; // Object sprite image
        unsigned short animTimer;
    public:
        // Methods
        Object();
        virtual ~Object();
        ALLEGRO_BITMAP* getImg();
        std::string getName();
        float getX();
        float getY();
        void setX(float _x);
        void setY(float _y);
        int getCurrTileCol();
        int getCurrTileRow();
        float getSpeed();
        void setSpeed(float _speed);
        int getDir();
        void setDir(int _dir);
        int lookAhead(int _dir, int _row, int _col);
        short getAnimTimer();
        void setAnimTimer(short _animTimer);
        bool getOutOfBounds();
        void setOutOfBounds(bool _value);
        // Subclass virtual methods
        virtual short getStartX() { return 0; };
        virtual short getStartY() { return 0; };
        virtual int getSize() { return 0; };
        virtual short getNoFrames() { return 0; };
        // Player specific virtual methods
        virtual float getLastSpeed() { return 0; };
        virtual void setLastSpeed(float _speed) {};
        virtual int getScore() { return 0; };
        virtual void setScore(int _score) {};
        virtual short getLives() { return 0; };
        virtual void setLives(short _lives) {};
        virtual bool isPlaying() { return 0; };
        virtual void setPlaying(bool _value) {};
        // Enemy specific virtual methods
        virtual std::vector<unsigned short> getTarget() { std::vector<unsigned short> _vector; return _vector; };
        virtual void setTarget(short _row, short _col) {};
        virtual std::string getMode() { return 0; };
        virtual void setMode(std::string _mode) {};
        virtual void findPath() {};
        virtual float checkDistance(float x0, float y0, float x1, float y1) { return 0; };
        virtual void setPrevTileRow(short _row) {};
        virtual short getPrevTileRow() { return 0; };
        virtual void setPrevTileCol(short _col) {};
        virtual short getPrevTileCol() { return 0; };
};

#endif // OBJECT_H
