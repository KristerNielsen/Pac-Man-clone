#include "Collision.hpp"
#include "Object.hpp"
#include "Game.hpp"

// Normal collision check
bool Collision::check(Object* _obj) {
    // TUNNEL REPOSITIONING
    // If heading out of the left tunnel
    if(_obj->getX() <= (Game::getWIDTH() - Game::getWIDTH()) - 60) {
        _obj->setX(Game::getWIDTH() + 59);
        _obj->setDir(3);
    }
    // If heading out of the right tunnel
    else if(_obj->getX() >= Game::getWIDTH() + 60) {
        _obj->setX((Game::getWIDTH() - Game::getWIDTH()) - 59);
        _obj->setDir(1);
    }
    // SPECIAL CHECK
    // When entering or exiting tunnels
    else if((Game::getTileValue(_obj) == 37 && _obj->getDir() == 3) || (Game::getTileValue(_obj) == 38 && _obj->getDir() == 1)) {
        _obj->setOutOfBounds(true);
    }
    // WIDE CHECK
    // If dot, energizer pill or empty space then there is no danger
    else if(!_obj->getOutOfBounds() &&
            _obj->lookAhead(_obj->getDir(), _obj->getCurrTileRow(), _obj->getCurrTileCol()) == 0 ||
            _obj->lookAhead(_obj->getDir(), _obj->getCurrTileRow(), _obj->getCurrTileCol()) == 1 ||
            _obj->lookAhead(_obj->getDir(), _obj->getCurrTileRow(), _obj->getCurrTileCol()) == 2 ||
            _obj->lookAhead(_obj->getDir(), _obj->getCurrTileRow(), _obj->getCurrTileCol()) == 37 ||
            _obj->lookAhead(_obj->getDir(), _obj->getCurrTileRow(), _obj->getCurrTileCol()) == 38 ||
            _obj->lookAhead(_obj->getDir(), _obj->getCurrTileRow(), _obj->getCurrTileCol()) == 39) {
        _obj->setOutOfBounds(false);
        // Return false if no collision
        return false;
    }
    // NARROW CHECK
    // Else we need to start take a closer look at how far the object can move before it stops
    else if(!_obj->getOutOfBounds())  {
        // Heading up
        // Wait until the object is above the center of the tile before...
        if(_obj->getDir() == 0 && ((int)_obj->getY() % Game::getTILE_SIZE()) <= 12 ) {
            // setting it to the center of the tile in Y-axis
            _obj->setY(((_obj->getCurrTileRow() + 1) * Game::getTILE_SIZE()) - 12);
            // If the collision check is for the player, set its speed to zero and
            // remember the last speed so that the animation can halt and then continue
            if(_obj->getName() == "P1" || _obj->getName() == "P2") setSpeedToZeroOnCollision(_obj);
            return true;
        }
        // Heading right
        // Wait until the object is to the right of the center of the tile before...
        if(_obj->getDir() == 1 && ((int)_obj->getX() % Game::getTILE_SIZE()) >= 12 ) {
            // setting it to the center of the tile in X-axis
            _obj->setX(((_obj->getCurrTileCol() + 1) * Game::getTILE_SIZE()) - 12);
            // If the collision check is for the player, set its speed to zero and
            // remember the last speed so that the animation can halt and then continue
            if(_obj->getName() == "P1" || _obj->getName() == "P2") setSpeedToZeroOnCollision(_obj);
            return true;
        }
        // Heading down
        // Wait until the object is below the center of the tile before...
        if(_obj->getDir() == 2 && ((int)_obj->getY() % Game::getTILE_SIZE()) >= 12 ) {
            // setting it to the center of the tile in Y-axis
            _obj->setY(((_obj->getCurrTileRow() + 1) * Game::getTILE_SIZE()) - 12);
            // If the collision check is for the player, set its speed to zero and
            // remember the last speed so that the animation can halt and then continue
            if(_obj->getName() == "P1" || _obj->getName() == "P2") setSpeedToZeroOnCollision(_obj);
            return true;
        }
        // Heading left
        // Wait until the object is to the left of the center of the tile before...
        if(_obj->getDir() == 3 && ((int)_obj->getX() % Game::getTILE_SIZE()) <= 12 ) {
            // setting it to the center of the tile in X-axis
            _obj->setX(((_obj->getCurrTileCol() + 1) * Game::getTILE_SIZE()) - 12);
            // If the collision check is for the player, set its speed to zero and
            // remember the last speed so that the animation can halt and then continue
            if(_obj->getName() == "P1" || _obj->getName() == "P2") setSpeedToZeroOnCollision(_obj);
            return true;
        }
        _obj->setOutOfBounds(false);
    }
}

// Collision in selected direction
bool Collision::checkInInputDir(Object* _obj, int _dir) {
    // If dot, energizer pill or empty space then there is no danger
    if(!_obj->getOutOfBounds() &&
        _obj->lookAhead(_dir, _obj->getCurrTileRow(), _obj->getCurrTileCol()) == 0 ||
        _obj->lookAhead(_dir, _obj->getCurrTileRow(), _obj->getCurrTileCol()) == 1 ||
        _obj->lookAhead(_dir, _obj->getCurrTileRow(), _obj->getCurrTileCol()) == 2 ||
        _obj->lookAhead(_dir, _obj->getCurrTileRow(), _obj->getCurrTileCol()) == 37 ||
        _obj->lookAhead(_dir, _obj->getCurrTileRow(), _obj->getCurrTileCol()) == 38 ||
        _obj->lookAhead(_dir, _obj->getCurrTileRow(), _obj->getCurrTileCol()) == 39) {
        // Make sure the object is close to the center of any tile when doing the check
        if( ((int)_obj->getX() % Game::getTILE_SIZE() > 9) &&
            ((int)_obj->getX() % Game::getTILE_SIZE() < 14) &&
            ((int)_obj->getY() % Game::getTILE_SIZE() > 9) &&
            ((int)_obj->getY() % Game::getTILE_SIZE() < 14) ) {
            // Return false if no collision
            return false;
        } else {
            return true;
        }
    } else {
        return true;
    }
}

void Collision::setSpeedToZeroOnCollision(Object* _obj) {
    if(_obj->getSpeed() > 0) _obj->setLastSpeed(_obj->getSpeed());
    _obj->setSpeed(0);
}
