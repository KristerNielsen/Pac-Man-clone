#ifndef PLAYER_H
#define PLAYER_H

#include "Object.hpp"

class Game;

class Player : public Object {
        // Properties
        const unsigned int SIZE = 39;
        const unsigned short NO_FRAMES = 7;
        float lastSpeed;
        unsigned int score;
        unsigned short lives;
        bool playing;
    public:
        // Methods
        Player(std::string _name);
        virtual ~Player();
        short getStartX();
        short getStartY();
        int getSize();
        short getNoFrames();
        float getLastSpeed();
        void setLastSpeed(float _speed);
        int getScore();
        void setScore(int _score);
        short getLives();
        void setLives(short _lives);
        bool isPlaying();
        void setPlaying(bool _value);
};

#endif // PLAYER_H
