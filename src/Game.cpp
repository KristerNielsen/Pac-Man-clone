#include "Game.hpp"
#include "Object.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Collision.hpp"
#include "State.hpp"
#include "Audio.hpp"

// ========================
// VARIABLES

// Static variable definitions
const unsigned short Game::WIDTH = 672;
const unsigned short Game::HEIGHT = 864;
const unsigned short Game::FPS = 60;
const unsigned short Game::TILE_SIZE = 24;
std::string Game::screen = "attract";
std::vector<std::vector<unsigned short>> Game::layout;
ALLEGRO_BITMAP* Game::tileImg;
ALLEGRO_BITMAP* Game::hudImg;
unsigned short Game::timer = 0;
unsigned short Game::credits = 0;
Object* Game::activePlayer = nullptr;
unsigned short Game::noPlayers = 0;
std::vector<Object*> Game::home = {};
unsigned short Game::enemyModeTimer = 0;

// Local variables
const char* player1 = "1UP";
const short textOffsetX = -3; // Sets the offset to 1 pixel off times 3 since the graphics has been scaled three times
const short textOffsetY = 0;
enum KEYS { UP, RIGHT, DOWN, LEFT }; // 0, 1, 2, 3

// ========================

// ========================
// METHODS

void Game::init() {
    // Variable init
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_EVENT_QUEUE* eventQueue = NULL;
    ALLEGRO_TIMER* timer = NULL;

    bool redraw = true;
    bool key[4] = { false, false, false, false };

    // ========================
    // ERROR HANDLING

    // Initialize Allegro
    al_init();

    // Create a display (window) sized in pixels
    display = al_create_display(getWIDTH(), getHEIGHT());

    // Initializes the image addon which is needed to display images
    al_init_image_addon();

    // Install the Allegro keyboard interrupt handler
    al_install_keyboard();

    // Create an event queue object
    eventQueue = al_create_event_queue();

    // Initialize the font addon
    al_init_font_addon();

    // Initialize the ttf (True Type Font) addon
    al_init_ttf_addon();

    // Create a timer object that will trigger every 1/60 seconds
    timer = al_create_timer(1.0 / getFPS());

    // Initialize audio
    Audio::init();

    // ========================

    // ========================
    // CREATE OBJECTS

    // Players
    Player* player1 = new Player("P1");
    Player* player2 = new Player("P2");

    // Shadow aka "Blinky"
    Enemy* blinky = new Enemy("BLINKY");
    // Speedy aka "Pinky"
    Enemy* pinky = new Enemy("PINKY");
    home.push_back(pinky);
    // Bashful aka "Inky"
    Enemy* inky = new Enemy("INKY");
    home.push_back(inky);
    // Pokey aka "Clyde"
    Enemy* clyde = new Enemy("CLYDE");
    home.push_back(clyde);

    // Set screen to attract mode
    setScreen("attract");

    // Create font. 30 px equals about the same size as in the original
    ALLEGRO_FONT* font = al_load_ttf_font("fonts/font.ttf", 24, ALLEGRO_TTF_NO_KERNING);

    // ========================

    // Tie objects to the event queue
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    // Start the timer
    al_start_timer(timer);

    while(1)
    {
        // Game loop
        ALLEGRO_EVENT event;
        // Checks the event queue for any type of event. If nothing comes in it will wait forever
        // but if a match is found, it will be copied into "event" and removed from the queue
        al_wait_for_event(eventQueue, &event);

        // ========================
        // EVENT HANDLING

        // Keyboard down
        if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[UP] = true;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[DOWN] = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    key[LEFT] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    key[RIGHT] = true;
                    break;
            }
        }
        // Keyboard up
        else if(event.type == ALLEGRO_EVENT_KEY_UP) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[UP] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[DOWN] = false;
                    break;
                case ALLEGRO_KEY_LEFT:
                    key[LEFT] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    key[RIGHT] = false;
                    break;
                // Player one start
                case ALLEGRO_KEY_1:
                    if(getScreen() == "credit") {
                        setNoPlayers(1);
                        setActivePlayer(player1);
                        setScreen("pregame");
                    }
                    break;
                // Two player start
                case ALLEGRO_KEY_2:
                    if(getScreen() == "credit") {
                        setNoPlayers(2);
                        setActivePlayer(player1);
                        setScreen("pregame");
                    }
                    break;
                // Add credit
                case ALLEGRO_KEY_5:
                    // Play credit audio sample each time the player presses the 5
                    Audio::play("credit");
                    setCredits(1);
                    if(getScreen() == "attract") setScreen("credit");
                    break;
            }
        }

        // Timer
        if(event.type == ALLEGRO_EVENT_TIMER) {

            redraw = true;

            if(getScreen() == "game") {
                // ========================
                // PLAYER MOVEMENT

                // Heading up
                if(activePlayer->getDir() == 0) {
                    // If no collision
                    if(!Collision::check(activePlayer)) {
                        activePlayer->setY(activePlayer->getY() - activePlayer->getSpeed());
                    }
                }
                // Heading right
                if(activePlayer->getDir() == 1) {
                    // If no collision
                    if(!Collision::check(activePlayer)) {
                        activePlayer->setX(activePlayer->getX() + activePlayer->getSpeed());
                    }
                }
                // Heading down
                if(activePlayer->getDir() == 2) {
                    // If no collision
                    if(!Collision::check(activePlayer)) {
                        activePlayer->setY(activePlayer->getY() + activePlayer->getSpeed());
                    }
                }
                // Heading left
                if(activePlayer->getDir() == 3) {
                    // If no collision
                    if(!Collision::check(activePlayer)) {
                        activePlayer->setX(activePlayer->getX() - activePlayer->getSpeed());
                    }
                }
                // Set a new direction only if the object is allowed to. This is done to prevent clipping.
                // If heading down
                if((key[UP] && activePlayer->getDir() == 2) || (key[UP] && !Collision::checkInInputDir(activePlayer, 0))) {
                    activePlayer->setDir(0);
                    if(activePlayer->getLastSpeed() > -1) activePlayer->setSpeed(activePlayer->getLastSpeed());
                // If heading left
                } else if((key[RIGHT] && activePlayer->getDir() == 3) || (key[RIGHT] && !Collision::checkInInputDir(activePlayer, 1))) {
                    activePlayer->setDir(1);
                    if(activePlayer->getLastSpeed() > -1) activePlayer->setSpeed(activePlayer->getLastSpeed());
                // If heading up
                } else if((key[DOWN] && activePlayer->getDir() == 0) || (key[DOWN] && !Collision::checkInInputDir(activePlayer, 2))) {
                    activePlayer->setDir(2);
                    if(activePlayer->getLastSpeed() > -1) activePlayer->setSpeed(activePlayer->getLastSpeed());
                // If heading right
                } else if((key[LEFT] && activePlayer->getDir() == 1) || (key[LEFT] && !Collision::checkInInputDir(activePlayer, 3))) {
                    activePlayer->setDir(3);
                    if(activePlayer->getLastSpeed() > -1) activePlayer->setSpeed(activePlayer->getLastSpeed());
                }
                // ========================

                // ========================
                // GHOST MODES

                // Scatter
                if(enemyModeTimer <= (7 * getFPS())) {
                    // Change mode only once
                    static bool initialized = false;
                    if(!initialized) {
                        blinky->setMode("scatter");
                        //blinky->setTarget(5, 27); // Top right corner
                        //blinky->setTarget(4, 1); // Top left corner
                        //blinky->setTarget(17, 1); // Left tunnel
                        blinky->setTarget(32, 1); // Bottom left corner
                        //blinky->setTarget(32, 27); // Bottom right corner
                        pinky->setMode("scatter");
                        inky->setMode("scatter");
                        clyde->setMode("scatter");
                        initialized = true;
                    }
                }
                // Chase
                if(enemyModeTimer > (7 * getFPS()) && enemyModeTimer <= (27 * getFPS())) {
                    // Change mode only once
                    static bool initialized = false;
                    if(!initialized) {
                        blinky->setMode("chase");
                        pinky->setMode("chase");
                        inky->setMode("chase");
                        clyde->setMode("chase");
                        initialized = true;
                    }
                }
                // ========================

                // ========================
                // GHOST MOVEMENT

                /*if(!home.empty()) {
                    // WHO IS *NOT* IN THE GHOST HOUSE?
                    // Search for all objects that are not at home and move them
                    if(std::find(home.begin(), home.end(), blinky) == home.end()) {
                        blinky->setTarget(5, 27);
                        // If no collision
                        if(!Collision::check(blinky)) {
                            blinky->setX(blinky->getX() - blinky->getSpeed());
                        }
                    }
                    if(std::find(home.begin(), home.end(), pinky) == home.end()) {
                        pinky->setX(pinky->getX() - pinky->getSpeed());
                    }
                    if(std::find(home.begin(), home.end(), inky) == home.end()) {
                        inky->setX(inky->getX() - inky->getSpeed());
                    }
                    if(std::find(home.begin(), home.end(), clyde) == home.end()) {
                        clyde->setX(clyde->getX() - clyde->getSpeed());
                    }
                    // WHO *IS* IN THE GHOST HOUSE?
                }*/

                // BLINKY
                blinky->findPath();
                // Heading up
                if(blinky->getDir() == 0) blinky->setY(blinky->getY() - blinky->getSpeed());
                // Heading right
                if(blinky->getDir() == 1) blinky->setX(blinky->getX() + blinky->getSpeed());
                // Heading down
                if(blinky->getDir() == 2) blinky->setY(blinky->getY() + blinky->getSpeed());
                // Heading left
                if(blinky->getDir() == 3) blinky->setX(blinky->getX() - blinky->getSpeed());
            }
            // ========================

            // Increment timer
            enemyModeTimer++;
        }
        // Close by using the X
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        // ==============

        // If a redraw has been triggered we want to wait until there are no more events in queue.
        // This is to prevent the update loop from falling too far behind
        if(redraw && al_is_event_queue_empty(eventQueue)) {
            redraw = false;
            // Clears the screen before each redraw
            al_clear_to_color(al_map_rgb(0,0,0));
            // Draw objects
            drawObjects(font, activePlayer, blinky, pinky, inky, clyde);
            // Flip the back buffer display into view
            al_flip_display();
        }
    } // While loop end

    // On exit, free up dynamic memory
    al_destroy_display(display);
    al_destroy_event_queue(eventQueue);
    al_destroy_timer(timer);

    delete(player1);
    delete(player2);
    //delete(screen);
}

short Game::getWIDTH() {
    return WIDTH;
}

short Game::getHEIGHT(){
    return HEIGHT;
}

double Game::getFPS() {
    return (double)FPS;
}

short Game::getTILE_SIZE() {
    return TILE_SIZE;
}

std::string Game::getScreen() {
    return screen;
}

void Game::setScreen(std::string _name) {
    hudImg = al_load_bitmap("sprites/sprite_hud.png");
    tileImg = al_load_bitmap("sprites/sprite_tiles.png");
    screen = _name;
    // Reset the timer each time a new screen is selected
    timer = 0;
    // ATTRACT
    if(_name == "attract") {
        layout = {};
    }
    // CREDIT
    else if(_name == "credit") {
        layout = {};
    }
    // PRE-GAME & GAME
    else if(_name == "pregame" || _name == "game") {
        enemyModeTimer = 0;
        layout =    {   // 28 wide and 36 high
                        { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
                        { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
                        { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
                        {  3,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4, 11, 12,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  5},
                        {  6,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 23, 24,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  7},
                        {  6,  1, 17, 18, 18, 19,  1, 17, 18, 18, 18, 19,  1, 23, 24,  1, 17, 18, 18, 18, 19,  1, 17, 18, 18, 19,  1,  7},
                        {  6,  2, 23, 32, 32, 24,  1, 23, 32, 32, 32, 24,  1, 23, 24,  1, 23, 32, 32, 32, 24,  1, 23, 32, 32, 24,  2,  7},
                        {  6,  1, 20, 21, 21, 22,  1, 20, 21, 21, 21, 22,  1, 20, 22,  1, 20, 21, 21, 21, 22,  1, 20, 21, 21, 22,  1,  7},
                        {  6,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  7},
                        {  6,  1, 17, 18, 18, 19,  1, 17, 19,  1, 17, 18, 18, 18, 18, 18, 18, 19,  1, 17, 19,  1, 17, 18, 18, 19,  1,  7},
                        {  6,  1, 20, 21, 21, 22,  1, 23, 24,  1, 20, 21, 21, 33, 34, 21, 21, 22,  1, 23, 24,  1, 20, 21, 21, 22,  1,  7},
                        {  6,  1,  1,  1,  1,  1,  1, 23, 24,  1,  1,  1,  1, 23, 24,  1,  1,  1,  1, 23, 24,  1,  1,  1,  1,  1,  1,  7},
                        {  8,  9,  9,  9,  9, 19,  1, 23, 35, 18, 18, 19,  0, 23, 24,  0, 17, 18, 18, 36, 24,  1, 17,  9,  9,  9,  9, 10},
                        { 32, 32, 32, 32, 32,  6,  1, 23, 34, 21, 21, 22,  0, 20, 22,  0, 20, 21, 21, 33, 24,  1,  7, 32, 32, 32, 32, 32},
                        { 32, 32, 32, 32, 32,  6,  1, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 23, 24,  1,  7, 32, 32, 32, 32, 32},
                        { 32, 32, 32, 32, 32,  6,  1, 23, 24,  0, 25,  9, 26, 27, 27, 28,  9, 29,  0, 23, 24,  1,  7, 32, 32, 32, 32, 32},
                        {  4,  4,  4,  4,  4, 22,  1, 20, 22,  0,  7,  0,  0,  0,  0,  0,  0,  6,  0, 20, 22,  1, 20,  4,  4,  4,  4,  4},
                        { 37,  0,  0,  0,  0,  0,  1,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  1,  0,  0,  0,  0,  0, 38},
                        {  9,  9,  9,  9,  9, 19,  1, 17, 19,  0,  7,  0,  0,  0,  0,  0,  0,  6,  0, 17, 19,  1, 17,  9,  9,  9,  9,  9},
                        { 32, 32, 32, 32, 32,  6,  1, 23, 24,  0, 30,  4,  4,  4,  4,  4,  4, 31,  0, 23, 24,  1,  7, 32, 32, 32, 32, 32},
                        { 32, 32, 32, 32, 32,  6,  1, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 23, 24,  1,  7, 32, 32, 32, 32, 32},
                        { 32, 32, 32, 32, 32,  6,  1, 23, 24,  0, 17, 18, 18, 18, 18, 18, 18, 19,  0, 23, 24,  1,  7, 32, 32, 32, 32, 32},
                        {  3,  4,  4,  4,  4, 22,  1, 20, 22,  0, 20, 21, 21, 33, 34, 21, 21, 22,  0, 20, 22,  1, 20,  4,  4,  4,  4,  5},
                        {  6,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 23, 24,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  7},
                        {  6,  1, 17, 18, 18, 19,  1, 17, 18, 18, 18, 19,  1, 23, 24,  1, 17, 18, 18, 18, 19,  1, 17, 18, 18, 19,  1,  7},
                        {  6,  1, 20, 21, 33, 24,  1, 20, 21, 21, 21, 22,  1, 20, 22,  1, 20, 21, 21, 21, 22,  1, 23, 34, 21, 22,  1,  7},
                        {  6,  2,  1,  1, 23, 24,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  1, 23, 24,  1,  1,  2,  7},
                        { 13, 18, 19,  1, 23, 24,  1, 17, 19,  1, 17, 18, 18, 18, 18, 18, 18, 19,  1, 17, 19,  1, 23, 24,  1, 17, 18, 15},
                        { 14, 21, 22,  1, 20, 22,  1, 23, 24,  1, 20, 21, 21, 33, 34, 21, 21, 22,  1, 23, 24,  1, 20, 22,  1, 20, 21, 16},
                        {  6,  1,  1,  1,  1,  1,  1, 23, 24,  1,  1,  1,  1, 23, 24,  1,  1,  1,  1, 23, 24,  1,  1,  1,  1,  1,  1,  7},
                        {  6,  1, 17, 18, 18, 18, 18, 36, 35, 18, 18, 19,  1, 23, 24,  1, 17, 18, 18, 36, 35, 18, 18, 18, 18, 19,  1,  7},
                        {  6,  1, 20, 21, 21, 21, 21, 21, 21, 21, 21, 22,  1, 20, 22,  1, 20, 21, 21, 21, 21, 21, 21, 21, 21, 22,  1,  7},
                        {  6,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  7},
                        {  8,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 10},
                        { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
                        { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}
                    };
    }
}

ALLEGRO_BITMAP* Game::getImg() {
    return tileImg;
}

Object* Game::getActivePlayer() {
    return activePlayer;
}

void Game::setActivePlayer(Object* _player) {
    activePlayer = _player;
}

short Game::getNoPlayers() {
    return noPlayers;
}

void Game::setNoPlayers(short _amount) {
    noPlayers = _amount;
}

void Game::update(Object* _obj) {
    // Update routines for the player
    if(_obj->getName() == "P1" || _obj->getName() == "P2") {
        // If dot
        if(layout[_obj->getCurrTileRow()][_obj->getCurrTileCol()] == 1) {
            layout[_obj->getCurrTileRow()][_obj->getCurrTileCol()] = 0;
            Audio::play("eat");
            // Dots are worth 10 points
            _obj->setScore(_obj->getScore() + 10);
        }
        // If pill
        else if(layout[_obj->getCurrTileRow()][_obj->getCurrTileCol()] == 2 || layout[_obj->getCurrTileRow()][_obj->getCurrTileCol()] == 39) {
            layout[_obj->getCurrTileRow()][_obj->getCurrTileCol()] = 0;
            Audio::play("eat");
            // Pills are worth 50 points
            _obj->setScore(_obj->getScore() + 50);
        }
    }
}

void Game::drawMap() {
    // "Height" of vector 3
    if(layout.size() != 0) {
        for(int i = 0; i < layout.size(); ++i) {
            // "Width" of vector 28
            for(int j = 0; j < layout[0].size(); ++j) {
                // Blink routine for energizer pills is done while the draw map
                // method is called and is based on a timer. 15 = 0.25 sec at 60 FPS
                if(getScreen() == "game" && timer % 15 == 14 && (layout[i][j] == 2 || layout[i][j] == 39)) {
                    // Ternary operation making it a toggle
                    layout[i][j] = (layout[i][j] == 2) ? 39 : 2;
                }
                // source x, source y, source width, source height, destination x, destination y
                al_draw_bitmap_region(tileImg, layout[i][j]*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, j*TILE_SIZE, i*TILE_SIZE, 0);
            }
        }
    }
}

short Game::getTileValue(int _row, int _col) {
    return layout[_row][_col];
}

short Game::getTileValue(Object* _obj) {
    return layout[_obj->getCurrTileRow()][_obj->getCurrTileCol()];
}

void Game::drawObjects(ALLEGRO_FONT* _font, Object* _player, Object* _blinky, Object* _pinky, Object* _inky, Object* _clyde) {

    // ========================
    // ATTRACT SCREEN
    if(getScreen() == "attract") {
        // 1UP
        al_draw_text(_font, al_map_rgb(255,255,255), 75 + textOffsetX, 0 + textOffsetY, ALLEGRO_ALIGN_LEFT, "1UP");
        al_draw_text(_font, al_map_rgb(255,255,255), 171 + textOffsetX, 27 + textOffsetY, ALLEGRO_ALIGN_RIGHT, "00");
        // HIGH SCORE
        al_draw_text(_font, al_map_rgb(255,255,255), 219 + textOffsetX, 0 + textOffsetY, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        // 2UP
        al_draw_text(_font, al_map_rgb(255,255,255), 531 + textOffsetX, 0 + textOffsetY, ALLEGRO_ALIGN_LEFT, "2UP");
        // CHARACTER / NICKNAME
        al_draw_text(_font, al_map_rgb(255,255,255), 171 + textOffsetX, 120 + textOffsetY, ALLEGRO_ALIGN_LEFT, "CHARACTER / NICKNAME");
        // CREDIT
        al_draw_textf(_font, al_map_rgb(255,255,255), 51 + textOffsetX, 840 + textOffsetY, ALLEGRO_ALIGN_LEFT, "CREDIT  %d", getCredits());

        // RED ENEMY
        if(timer >= 60) {
            al_draw_bitmap_region(_blinky->getImg(), 3 * _blinky->getSize(), 0, _blinky->getSize(), _blinky->getSize(), 99, 156, 0);
        }
        // SHADOW TEXT
        if(timer >= 120) {
            al_draw_text(_font, al_map_rgb(255,0,0), 171 + textOffsetX, 168 + textOffsetY, ALLEGRO_ALIGN_LEFT, "-SHADOW");
        }
        // "BLINKY" TEXT
        if(timer >= 150) {
            al_draw_text(_font, al_map_rgb(255,0,0), 435 + textOffsetX, 168 + textOffsetY, ALLEGRO_ALIGN_LEFT, "\"BLINKY\"");
        }

        // PINK ENEMY
        if(timer >= 180) {
            al_draw_bitmap_region(_pinky->getImg(), 11 * _pinky->getSize(), 0, _pinky->getSize(), _pinky->getSize(), 99, 228, 0);
        }
        // SPEEDY TEXT
        if(timer >= 240) {
            al_draw_text(_font, al_map_rgb(255,184,255), 171 + textOffsetX, 240 + textOffsetY, ALLEGRO_ALIGN_LEFT, "-SPEEDY");
        }
        // "PINKY" TEXT
        if(timer >= 270) {
            al_draw_text(_font, al_map_rgb(255,184,255), 435 + textOffsetX, 240 + textOffsetY, ALLEGRO_ALIGN_LEFT, "\"PINKY\"");
        }

        // BLUE ENEMY
        if(timer >= 300) {
            al_draw_bitmap_region(_inky->getImg(), 19 * _inky->getSize(), 0, _inky->getSize(), _inky->getSize(), 99, 300, 0);
        }
        // BASHFUL TEXT
        if(timer >= 360) {
            al_draw_text(_font, al_map_rgb(0,255,255), 171 + textOffsetX, 312 + textOffsetY, ALLEGRO_ALIGN_LEFT, "-BASHFUL");
        }
        // "INKY" TEXT
        if(timer >= 390) {
            al_draw_text(_font, al_map_rgb(0,255,255), 435 + textOffsetX, 312 + textOffsetY, ALLEGRO_ALIGN_LEFT, "\"INKY\"");
        }

        // ORANGE ENEMY
        if(timer >= 420) {
            al_draw_bitmap_region(_clyde->getImg(), 27 * _clyde->getSize(), 0, _clyde->getSize(), _clyde->getSize(), 99, 372, 0);
        }
        // POKEY TEXT
        if(timer >= 480) {
            al_draw_text(_font, al_map_rgb(255,184,81), 171 + textOffsetX, 384 + textOffsetY, ALLEGRO_ALIGN_LEFT, "-POKEY");
        }
        // "CLYDE" TEXT
        if(timer >= 510) {
            al_draw_text(_font, al_map_rgb(255,184,81), 435 + textOffsetX, 384 + textOffsetY, ALLEGRO_ALIGN_LEFT, "\"CLYDE\"");
        }

        // DOT
        if(timer >= 570) {
            al_draw_bitmap_region(tileImg, 1*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, 240, 576, 0);
        // 10 PTS TEXT, uses "a", "b" and "c" for custom font text
            al_draw_text(_font, al_map_rgb(255,255,255), 291 + textOffsetX, 576 + textOffsetY, ALLEGRO_ALIGN_LEFT, "10 abc");
        // 50 PTS TEXT
            al_draw_text(_font, al_map_rgb(255,255,255), 291 + textOffsetX, 624 + textOffsetY, ALLEGRO_ALIGN_LEFT, "50 abc");
        }

        // COPYRIGHT TEXT
        if(timer >= 630) {
            al_draw_text(_font, al_map_rgb(255,184,255), 99 + textOffsetX, 744 + textOffsetY, ALLEGRO_ALIGN_LEFT, "@ 1980 MIDWAY MFG.CO.");

        }

        // PILLS
        if(timer >= 570) {
            al_draw_bitmap_region(tileImg, 2*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, 240, 624, 0);
        }
        if(timer >= 630) {
            al_draw_bitmap_region(tileImg, 2*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, 96, 480, 0);
        }

        // ANIMATION
        static short tile = 2;
        // Ternary operation making it a toggle
        if(timer % 15 == 0) tile = (tile == 2) ? 0 : 2;
        if(timer >= 690) {
            al_draw_bitmap_region(tileImg, tile*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, 240, 624, 0);
            al_draw_bitmap_region(tileImg, tile*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, 96, 480, 0);
        }
    }
    // ========================

    // ========================
    // CREDITS SCREEN
    else if(getScreen() == "credit") {
        // 1UP
        al_draw_text(_font, al_map_rgb(255,255,255), 75 + textOffsetX, 0 + textOffsetY, ALLEGRO_ALIGN_LEFT, "1UP");
        al_draw_text(_font, al_map_rgb(255,255,255), 171 + textOffsetX, 27 + textOffsetY, ALLEGRO_ALIGN_RIGHT, "00");
        // HIGH SCORE
        al_draw_text(_font, al_map_rgb(255,255,255), 219 + textOffsetX, 0 + textOffsetY, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        // 2UP
        al_draw_text(_font, al_map_rgb(255,255,255), 531 + textOffsetX, 0 + textOffsetY, ALLEGRO_ALIGN_LEFT, "2UP");

        // PUSH START BUTTON TEXT
        al_draw_text(_font, al_map_rgb(255,184,81), 147 + textOffsetX, 384 + textOffsetY, ALLEGRO_ALIGN_LEFT, "PUSH START BUTTON");

        // PLAYER TEXTS
        if(getCredits() == 1) {
            al_draw_text(_font, al_map_rgb(0,255,255), 195 + textOffsetX, 480 + textOffsetY, ALLEGRO_ALIGN_LEFT, "1 PLAYER ONLY");
        } else {
            al_draw_text(_font, al_map_rgb(0,255,255), 195 + textOffsetX, 480 + textOffsetY, ALLEGRO_ALIGN_LEFT, "1 OR 2 PLAYERS");
        }

        // BONUS PAC-MAN TEXT
        al_draw_text(_font, al_map_rgb(255,184,174), 27 + textOffsetX, 576 + textOffsetY, ALLEGRO_ALIGN_LEFT, "BONUS PAC-MAN FOR 10000 abc");

        // COPYRIGHT TEXT
        al_draw_text(_font, al_map_rgb(255,184,255), 99 + textOffsetX, 672 + textOffsetY, ALLEGRO_ALIGN_LEFT, "@ 1980 MIDWAY MFG.CO.");

        // CREDIT
        al_draw_textf(_font, al_map_rgb(255,255,255), 51 + textOffsetX, 840 + textOffsetY, ALLEGRO_ALIGN_LEFT, "CREDIT  %d", getCredits());

    }
    // ========================

    // ========================
    // PRE-GAME AND GAME SCREEN
    else if(getScreen() == "pregame" || getScreen() == "game") {
        // static
        //setCredits(-1);
        // Update screen
        if(_player->isPlaying()) update(_player);
        // Draw screen
        drawMap();

        // Blink routine for 1UP / 2UP. 24 = 0.4 sec at 60 FPS
        if(timer % 24 == 23) {
            if(_player->getName() == "player1")
            // Ternary operation making it a toggle
            player1 = (player1 == "1UP") ? "" : "1UP";
        }

        // 1UP
        al_draw_textf(_font, al_map_rgb(255,255,255), 75 + textOffsetX, 0 + textOffsetY, ALLEGRO_ALIGN_LEFT, "%s", player1);
        if(_player->getScore() == 0) al_draw_text(_font, al_map_rgb(255,255,255), 171 + textOffsetX, 27 + textOffsetY, ALLEGRO_ALIGN_RIGHT, "00");
        else al_draw_textf(_font, al_map_rgb(255,255,255), 171 + textOffsetX, 27 + textOffsetY, ALLEGRO_ALIGN_RIGHT, "%d", _player->getScore());
        // HIGH SCORE
        al_draw_text(_font, al_map_rgb(255,255,255), 219 + textOffsetX, 0 + textOffsetY, ALLEGRO_ALIGN_LEFT, "HIGH SCORE");
        // 2UP
        al_draw_text(_font, al_map_rgb(255,255,255), 531 + textOffsetX, 0 + textOffsetY, ALLEGRO_ALIGN_LEFT, "2UP");
        // 624px  player two score

        // PRE-GAME SCREEN
        if(!_player->isPlaying()) {
            // Timer events that show different elements at different times
            // STEP 1
            if(timer <= 130) {
                // Play pre-game audio sample
                Audio::play("pregame");
                al_draw_text(_font, al_map_rgb(0,255,255), 219 + textOffsetX, 336 + textOffsetY, ALLEGRO_ALIGN_LEFT, "PLAYER ONE");
                al_draw_text(_font, al_map_rgb(255,255,0), 267 + textOffsetX, 480 + textOffsetY, ALLEGRO_ALIGN_LEFT, "READY!");
            }
            // STEP 2
            else if(timer > 130 && timer <= 265) {
                al_draw_text(_font, al_map_rgb(255,255,0), 267 + textOffsetX, 480 + textOffsetY, ALLEGRO_ALIGN_LEFT, "READY!");
                // Draw player position, at its center
                al_draw_bitmap_region(_player->getImg(), 0, 0, _player->getSize(), _player->getSize(), _player->getX() - (_player->getSize() / 2), _player->getY() - (_player->getSize() / 2), 0);
                // Draw Blinky who starts of heading left
                al_draw_bitmap_region(_blinky->getImg(), (6 * _blinky->getSize()) + (_blinky->getSize() * 0), 0, _blinky->getSize(), _blinky->getSize(), _blinky->getX() - (_blinky->getSize() / 2), _blinky->getY() - (_blinky->getSize() / 2), 0);
                // Draw Pinky who starts of heading down
                al_draw_bitmap_region(_pinky->getImg(), (4 * _pinky->getSize()) + (_pinky->getSize() * 8), 0, _pinky->getSize(), _pinky->getSize(), _pinky->getX() - (_pinky->getSize() / 2), _pinky->getY() - (_pinky->getSize() / 2), 0);
                // Draw Inky who starts of heading up
                al_draw_bitmap_region(_inky->getImg(), (0 * _inky->getSize()) + (_inky->getSize() * 16), 0, _inky->getSize(), _inky->getSize(), _inky->getX() - (_inky->getSize() / 2), _inky->getY() - (_inky->getSize() / 2), 0);
                // Draw Clyde who starts of heading up
                al_draw_bitmap_region(_clyde->getImg(), (0 * _clyde->getSize()) + (_clyde->getSize() * 24), 0, _clyde->getSize(), _clyde->getSize(), _clyde->getX() - (_clyde->getSize() / 2), _clyde->getY() - (_clyde->getSize() / 2), 0);

                // Makes sure that the number of lives is only subtracted once
                static bool initialized = false;
                if (!initialized) { _player->setLives(_player->getLives() - 1); initialized = true; }
            }
            // GO! GO! GO!
            else if(timer > 265) {
                _player->setPlaying(true);
                setScreen("game");
            }
        // GAME SCREEN
        } else {
            // Draw player position, at its center
            al_draw_bitmap_region(_player->getImg(), State::animate(_player), 0, _player->getSize(), _player->getSize(), _player->getX() - (_player->getSize() / 2), _player->getY() - (_player->getSize() / 2), 0);

            // Draw Blinky
            al_draw_bitmap_region(_blinky->getImg(), State::animate(_blinky), 0, _blinky->getSize(), _blinky->getSize(), _blinky->getX() - (_blinky->getSize() / 2), _blinky->getY() - (_blinky->getSize() / 2), 0);
            // Draw Pinky
            al_draw_bitmap_region(_pinky->getImg(), State::animate(_pinky), 0, _pinky->getSize(), _pinky->getSize(), _pinky->getX() - (_pinky->getSize() / 2), _pinky->getY() - (_pinky->getSize() / 2), 0);
            // Draw Inky
            al_draw_bitmap_region(_inky->getImg(), State::animate(_inky), 0, _inky->getSize(), _inky->getSize(), _inky->getX() - (_inky->getSize() / 2), _inky->getY() - (_inky->getSize() / 2), 0);
            // Draw Clyde
            al_draw_bitmap_region(_clyde->getImg(), State::animate(_clyde), 0, _clyde->getSize(), _clyde->getSize(), _clyde->getX() - (_clyde->getSize() / 2), _clyde->getY() - (_clyde->getSize() / 2), 0);
        }

        // Draw lives
        for(int i = 0; i < _player->getLives(); i++) {
            al_draw_bitmap_region(hudImg, 0, 0, 36, 36, 54 + (i * 48), HEIGHT - 42, 0);
        }

        // Draw fruits
        for(int i = 0; i < _player->getLives() - 1; i++) {
            al_draw_bitmap_region(hudImg, 36, 0, 36, 36, WIDTH - 90, HEIGHT - 42, 0);
        }
    }
    // ========================

    // Increase the timer each frame and reset if it reaches FPS * 60 which is equivalent to 60 seconds or 1 minute at 60 FPS
    if(timer < getFPS() * 60) timer++;
    else timer = 0;
}

short Game::getCredits() {
    return credits;
}

void Game::setCredits(short _amount) {
    credits += _amount;
}

void Game::setEnemyModeTimer(short _value) {
    enemyModeTimer = _value;
}
//al_destroy_bitmap(tileImg);
// Reset timer when player dies

