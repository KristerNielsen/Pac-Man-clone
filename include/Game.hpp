#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

class Object;
class Player;
class Enemy;
class Collision;
class State;
class Audio;

class Game {
        // Properties
        static const unsigned short WIDTH;
        static const unsigned short HEIGHT;
        static const unsigned short FPS;
        static const unsigned short TILE_SIZE;
        static std::string screen;
        static std::vector<std::vector<unsigned short>> layout;
        static ALLEGRO_BITMAP* tileImg; // Tile sprite image
        static ALLEGRO_BITMAP* hudImg; // HUD sprite image
        static unsigned short timer;
        static unsigned short credits;
        static Object* activePlayer;
        static unsigned short noPlayers;
        // Enemy properties
        static std::vector<Object*> home;
        static unsigned short enemyModeTimer;
    public:
        // Methods
        static void init();
        static short getWIDTH();
        static short getHEIGHT();
        static double getFPS();
        static short getTILE_SIZE();
        static std::string getScreen();
        static void setScreen(std::string _name);
        static ALLEGRO_BITMAP* getImg();
        static Object* getActivePlayer();
        static void setActivePlayer(Object* _player);
        static short getNoPlayers();
        static void setNoPlayers(short _amount);
        static void update(Object* _obj);
        static void drawMap();
        static short getTileValue(int _row, int _col); // Used by object lookAhead function
        static short getTileValue(Object* _obj);
        static void drawObjects(ALLEGRO_FONT* _font, Object* _player, Object* _blinky, Object* _pinky, Object* _inky, Object* _clyde);
        static short getCredits();
        static void setCredits(short _amount);
        static void setEnemyModeTimer(short _value);
};

#endif // GAME_H
