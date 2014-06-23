#include "Enemy.hpp"
#include "Game.hpp"
#include "Collision.hpp"

Enemy::Enemy(std::string _name) {
    name = _name;
    if(_name == "BLINKY") {
            startX = 336;
            startY = 348;
            setDir(3);
            setPrevTileRow(14);
            setPrevTileCol(14);
            setMode("scatter");
    }
    else if(_name == "PINKY") {
            startX = 336;
            startY = 420;
            setDir(2);
            setMode("scatter");
    }
    else if(_name == "INKY") {
            startX = 288;
            startY = 420;
            setDir(0);
            setMode("idle");
    }
    else if(_name == "CLYDE") {
            startX = 384;
            startY = 420;
            setDir(0);
            setMode("idle");
    }
    setX(getStartX());
    setY(getStartY());
    setSpeed(10); // Start at 80% speed of normal
    img = al_load_bitmap("sprites/sprite_enemies.png");
    setOutOfBounds(false);
}

Enemy::~Enemy() {
}

short Enemy::getStartX() {
    return startX;
}

short Enemy::getStartY() {
    return startY;
}

int Enemy::getSize() {
    return SIZE;
}

short Enemy::getNoFrames() {
    return NO_FRAMES;
}

std::vector<unsigned short> Enemy::getTarget() {
    std::vector<unsigned short> temp = { targetX, targetY };
    return temp;
}

void Enemy::setTarget(short _row, short _col) {
    targetX = _col * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2);
    targetY = _row * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2);
}

std::string Enemy::getMode() {
    return mode;
}

void Enemy::setMode(std::string _mode) {
    Game::setEnemyModeTimer(0);
    mode = _mode;
}

void Enemy::findPath() {
    // 1. Check what direction the enemy is traveling in and remember the last tile she was on
    // 2. If she is free to move to any of the adjacent tiles, determine their individual distance to their target tile
    // 3. Set new direction towards the tile that has the shortest distance

    float up, right, down, left = 1000;

    // ========================
    // CHECK ADJACENT TILE DISTANCES

    // If heading up
    if(this->getDir() == 0) {

        // Set last tile visited
        if(this->getCurrTileRow() != this->getPrevTileRow()) this->setPrevTileRow(this->getCurrTileRow() + 1);
        if(this->getCurrTileCol() != this->getPrevTileCol()) this->setPrevTileCol(this->getCurrTileCol());

        // Check left
        if(!Collision::checkInInputDir(this, 3)) {
            // If no collision then get the distance between the tile and target
            left = checkDistance((this->getCurrTileCol() - 1) * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                this->getCurrTileRow() *  Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (float)this->getTarget()[0] , (float)this->getTarget()[1]);
        } else {
            left = 1000;
        }
        // Check up
        if(!Collision::checkInInputDir(this, 0)) {
            up = checkDistance(this->getCurrTileCol() * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (this->getCurrTileRow() - 1) *  Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (float)this->getTarget()[0] , (float)this->getTarget()[1]);
        } else {
            up = 1000;
        }
        // Check right
        if(!Collision::checkInInputDir(this, 1)) {

            right = checkDistance((this->getCurrTileCol() + 1) * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                this->getCurrTileRow() * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (float)this->getTarget()[0] , (float)this->getTarget()[1]);
        } else {
            right = 1000;
        }
        down = 1000;
    }
    // If heading right
    else if(getDir() == 1) {

        // Set last tile visited
        if(this->getCurrTileRow() != this->getPrevTileRow()) setPrevTileRow(this->getCurrTileRow());
        if(this->getCurrTileCol() != this->getPrevTileCol()) setPrevTileCol(this->getCurrTileCol() - 1);

        // Check up
        if(!Collision::checkInInputDir(this, 0)) {
            // If no collision then get the distance between the tile and target
            up = checkDistance(this->getCurrTileCol() * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (this->getCurrTileRow() - 1) *  Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (float)this->getTarget()[0] , (float)this->getTarget()[1]);
        } else {
            up = 1000;
        }
        // Check right
        if(!Collision::checkInInputDir(this, 1)) {
            right = checkDistance((this->getCurrTileCol() + 1) * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                this->getCurrTileRow() *  Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (float)this->getTarget()[0] , (float)this->getTarget()[1]);
        } else {
            right = 1000;
        }
        // Check down
        if(!Collision::checkInInputDir(this, 2)) {
            down = checkDistance(this->getCurrTileCol() * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (this->getCurrTileRow() + 1) * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (float)this->getTarget()[0] , (float)this->getTarget()[1]);
        } else {
            down = 1000;
        }
        left = 1000;
    }
    // If heading down
    else if(getDir() == 2) {

        // Set last tile visited
        if(this->getCurrTileRow() != this->getPrevTileRow()) setPrevTileRow(this->getCurrTileRow() - 1);
        if(this->getCurrTileCol() != this->getPrevTileCol()) setPrevTileCol(this->getCurrTileCol());

        // Check right
        if(!Collision::checkInInputDir(this, 1)) {
            right = checkDistance((this->getCurrTileCol() + 1) * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                this->getCurrTileRow() *  Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (float)this->getTarget()[0] , (float)this->getTarget()[1]);
        } else {
            right = 1000;
        }
        // Check down
        if(!Collision::checkInInputDir(this, 2)) {

            down = checkDistance(this->getCurrTileCol() * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (this->getCurrTileRow() + 1) * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (float)this->getTarget()[0] , (float)this->getTarget()[1]);
        } else {
            down = 1000;
        }
        // Check left
        if(!Collision::checkInInputDir(this, 3)) {
            left = checkDistance((this->getCurrTileCol() - 1) * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                this->getCurrTileRow() *  Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (float)this->getTarget()[0] , (float)this->getTarget()[1]);
        } else {
            left = 1000;
        }
        up = 1000;
    }
    // If heading left
    else if(getDir() == 3) {

        // Set last tile visited
        if(this->getCurrTileRow() != this->getPrevTileRow()) setPrevTileRow(this->getCurrTileRow());
        if(this->getCurrTileCol() != this->getPrevTileCol()) setPrevTileCol(this->getCurrTileCol() + 1);

        // Check down
        if(!Collision::checkInInputDir(this, 2)) {
            // If no collision then get the distance between the tile and target
            down = checkDistance(this->getCurrTileCol() * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (this->getCurrTileRow() + 1) *  Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (float)this->getTarget()[0] , (float)this->getTarget()[1]);
        } else {
            down = 1000;
        }
        // Check left
        if(!Collision::checkInInputDir(this, 3)) {
            left = checkDistance((this->getCurrTileCol() - 1) * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                this->getCurrTileRow() *  Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (float)this->getTarget()[0] , (float)this->getTarget()[1]);
        } else {
            left = 1000;
        }
        // Check up
        if(!Collision::checkInInputDir(this, 0)) {

            up = checkDistance((this->getCurrTileCol() - 1) * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                this->getCurrTileRow() * Game::getTILE_SIZE() - (Game::getTILE_SIZE() / 2),
                                (float)this->getTarget()[0] , (float)this->getTarget()[1]);
        } else {
            up = 1000;
        }
        right = 1000;
    }

    // ========================
    // CHECK TILES, SET DIRECTION

    // Which of the checked tiles has the shortest distance to the target
    float shortest = std::min(std::min(up, right), std::min(down, left));

std::cout << "===================================\n";
std::cout << "Shortest: " << shortest << "\n";
std::cout << "UP: " << up << " | RIGHT: " << right << " | DOWN: " << down << " | LEFT: " << left << "\n";
std::cout << "Current ROW: " << this->getCurrTileRow() << " | Current COL: " << this->getCurrTileCol() <<"\n";
std::cout << "Previous ROW: " << this->getPrevTileRow() << " | Previous COL: " << this->getPrevTileCol() <<"\n";
std::cout << "Direction: " << this->getDir() << "\n";

    if( shortest == up && up < 1000 &&
        this->getCurrTileRow() == this->getPrevTileRow() &&
        this->getCurrTileCol() != this->getPrevTileCol()) {
        setDir(0);
    }
    else if(shortest == right && right < 1000 &&
        this->getCurrTileRow() != this->getPrevTileRow() &&
        this->getCurrTileCol() == this->getPrevTileCol()) {
        setDir(1);
    }
    else if(shortest == down && down < 1000 &&
        this->getCurrTileRow() == this->getPrevTileRow() &&
        this->getCurrTileCol() != this->getPrevTileCol()) {
        setDir(2);
    }
    else if(shortest == left && left < 1000 &&
        this->getCurrTileRow() != this->getPrevTileRow() &&
        this->getCurrTileCol() == this->getPrevTileCol()) {
        setDir(3);
    }
}

float Enemy::checkDistance(float x0, float y0, float x1, float y1) {
    return sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

void Enemy::setPrevTileRow(short _row) {
    prevTileRow = _row;
}

short Enemy::getPrevTileRow() {
    return prevTileRow;
}

void Enemy::setPrevTileCol(short _col) {
    prevTileCol = _col;
}

short Enemy::getPrevTileCol() {
    return prevTileCol;
}
