#include "Player.hpp"
#include "Game.hpp"

Player::Player(std::string _name) {
    name = _name;
    startX = 336; // Start position for player is X = 336, Y = 636
    startY = 636;
    setX(getStartX());
    setY(getStartY());
    setSpeed(80); // Start at 80% speed of normal
    lastSpeed = -1; // lastSpeed remembers the last speed the player had when colliding with an object and stops
    setDir(3); // Start off going left
    img = al_load_bitmap("sprites/sprite_player.png");
    score = 0;
    lives = 3;
    playing = false;
    setOutOfBounds(false);
}

Player::~Player() {
    al_destroy_bitmap(img);
}

short Player::getStartX() {
    return startX;
}

short Player::getStartY() {
    return startY;
}

int Player::getSize() {
    return SIZE;
}

short Player::getNoFrames() {
    return NO_FRAMES;
}

float Player::getLastSpeed() {
    return lastSpeed;
}

void Player::setLastSpeed(float _speed) {
    // Revert last know speed back to percentage value (based on the Object::setSpeed algorithm)
    lastSpeed = ((getSpeed() * 100) * Game::getFPS()) / (9 * Game::getTILE_SIZE());
}

int Player::getScore() {
    return score;
}

void Player::setScore(int _score) {
    score = _score;
}

short Player::getLives() {
    return lives;
}

void Player::setLives(short _lives) {
    lives = _lives;
}

bool Player::isPlaying() {
    return playing;
}

void Player::setPlaying(bool _value) {
    playing = _value;
}
