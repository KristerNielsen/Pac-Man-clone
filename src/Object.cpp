#include "Object.hpp"
#include "Game.hpp"

Object::Object() {
    animTimer = 0;
}

Object::~Object() {
}

ALLEGRO_BITMAP* Object::getImg() {
    return img;
}

std::string Object::getName() {
    return name;
}

float Object::getX() {
    return x;
}

float Object::getY() {
    return y;
}

void Object::setX(float _x) {
    x = _x;
    // This method also sets the curent tile
    currTileCol = (int)(x / 24);
}

void Object::setY(float _y) {
    y = _y;
    // This method also sets the curent tile
    currTileRow = (int)(y / 24);
}

int Object::getCurrTileCol() {
    return currTileCol;
}

int Object::getCurrTileRow() {
    return currTileRow;
}

int Object::lookAhead(int _dir, int _row, int _col) {
    // Heading up
    if(_dir == 0) {
        return Game::getTileValue(_row - 1, _col);
    }
    // Heading right
    if(_dir == 1) {
        return Game::getTileValue(_row, _col + 1);
    }
    // Heading down
    if(_dir == 2) {
        return Game::getTileValue(_row + 1, _col);
    }
    // Heading left
    if(_dir == 3) {
        return Game::getTileValue(_row, _col - 1);
    }
}

float Object::getSpeed() {
    return speed;
}

void Object::setSpeed(float _speed) {
    // 11 tiles per sec (original Pac-Man speed) * 24 px wide tiles = 264 px per sec (9 pills per sec seems more accurate)
    // 264 / 60 FPS = ~4.4 px per frame
    // setSpeed(80) means 80% of normal speed
    speed = (_speed / 100) * ((9 * Game::getTILE_SIZE()) / Game::getFPS());
}

int Object::getDir() {
    return dir;
}

void Object::setDir(int _dir) {
    dir = _dir;
}

short Object::getAnimTimer() {
    return animTimer;
}

void Object::setAnimTimer(short _animTimer) {
    animTimer = _animTimer;
}

bool Object::getOutOfBounds() {
    return outOfBounds;
}

void Object::setOutOfBounds(bool _value) {
    outOfBounds = _value;
}
