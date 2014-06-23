#include "State.hpp"

short State::animate(Object* _obj) {
    // Increase the animation timer each tick
    // Player objects have a total of seven parts in their animation in any direction
    // Each enemy only have two animations in their loop when going in either direction
    if(_obj->getAnimTimer() < _obj->getNoFrames() && _obj->getSpeed() > 0) _obj->setAnimTimer(_obj->getAnimTimer() + 1);
    else _obj->setAnimTimer(0);

    // ========================
    // PLAYER ANIMATIONS

    if(_obj->getName() == "P1" || _obj->getName() == "P2") {
        // First frame is shared
        if(_obj->getAnimTimer() <= 3 && _obj->getSpeed() > 0) return 0 * _obj->getSize();
        // This line prevents the animation to stop on the "closed" frame
        else if(_obj->getSpeed() == 0) _obj->setAnimTimer(4);

        // Heading up
        if(_obj->getDir() == 0) {
            if(_obj->getAnimTimer() > 3 && _obj->getAnimTimer() <= 5) return 7 * _obj->getSize();
            else if(_obj->getAnimTimer() > 5 && _obj->getAnimTimer() <= 7) return 8 * _obj->getSize();
        }
        // Heading right
        else if(_obj->getDir() == 1) {
            if(_obj->getAnimTimer() > 3 && _obj->getAnimTimer() <= 5) return 1 * _obj->getSize();
            else if(_obj->getAnimTimer() > 5 && _obj->getAnimTimer() <= 7) return 2 * _obj->getSize();
        }
        // Heading down
        else if(_obj->getDir() == 2) {
            if(_obj->getAnimTimer() > 3 && _obj->getAnimTimer() <= 5) return 3 * _obj->getSize();
            else if(_obj->getAnimTimer() > 5 && _obj->getAnimTimer() <= 7) return 4 * _obj->getSize();
        }
        // Heading left
        else if(_obj->getDir() == 3) {
            if(_obj->getAnimTimer() > 3 && _obj->getAnimTimer() <= 5) return 5 * _obj->getSize();
            else if(_obj->getAnimTimer() > 5 && _obj->getAnimTimer() <= 7) return 6 * _obj->getSize();
        }
    }
    // ========================

    // ========================
    // ENEMY ANIMATIONS

    if(_obj->getName() == "BLINKY" || _obj->getName() == "PINKY" || _obj->getName() == "INKY" || _obj->getName() == "CLYDE") {

        // Offset to get the correct region of the image
        unsigned short offset = 0;

        // Set offsets
        if(_obj->getName() == "BLINKY") { offset = (_obj->getSize() * 0); }
        else if(_obj->getName() == "PINKY") { offset = (_obj->getSize() * 8); }
        else if(_obj->getName() == "INKY") { offset = (_obj->getSize() * 16); }
        else if(_obj->getName() == "CLYDE") { offset =(_obj->getSize() *  24); }

        // Heading up
        if(_obj->getDir() == 0) {
            if(_obj->getAnimTimer() <= 14) return (0 * _obj->getSize()) + offset;
            else if(_obj->getAnimTimer() > 14) return (1 * _obj->getSize()) + offset;
        }
        // Heading right
        else if(_obj->getDir() == 1) {
            if(_obj->getAnimTimer() <= 14) return (2 * _obj->getSize()) + offset;
            else if(_obj->getAnimTimer() > 14) return (3 * _obj->getSize()) + offset;
        }
        // Heading down
        else if(_obj->getDir() == 2) {
            if(_obj->getAnimTimer() <= 14) return (4 * _obj->getSize()) + offset;
            else if(_obj->getAnimTimer() > 14) return (5 * _obj->getSize()) + offset;
        }
        // Heading left
        else if(_obj->getDir() == 3) {
            if(_obj->getAnimTimer() <= 14) return (6 * _obj->getSize()) + offset;
            else if(_obj->getAnimTimer() > 14) return (7 * _obj->getSize()) + offset;
        }
    }
}

short State::death(Object* _obj) {
}
