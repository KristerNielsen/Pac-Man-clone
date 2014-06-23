#include "Audio.hpp"

ALLEGRO_SAMPLE* aCredit = nullptr;
ALLEGRO_SAMPLE* aPregame = nullptr;
ALLEGRO_SAMPLE* aEat1 = nullptr;
ALLEGRO_SAMPLE* aEat2 = nullptr;
bool playFirst = true;

void Audio::init() {

    if(!al_install_audio()){
        fprintf(stderr, "Failed to initialize audio!\n");
    }

    if(!al_init_acodec_addon()){
        fprintf(stderr, "Failed to initialize audio codecs!\n");
    }

    if (!al_reserve_samples(1)){
        fprintf(stderr, "Failed to reserve samples!\n");
    }

    // Credit
    aCredit = al_load_sample("audio/credit.wav");
    if (!aCredit){
        printf( "\"Credit\" audio sample not loaded!\n" );
    }

    // Pregame
    aPregame = al_load_sample("audio/pregame.wav");
    if (!aPregame){
        printf( "\"Pre-game\" audio sample not loaded!\n" );
    }

    // Eat1
    aEat1 = al_load_sample("audio/player_eating_1.wav");
    if (!aEat1){
        printf( "\"Eat1\" audio sample not loaded!\n" );
    }

    // Eat2
    aEat2 = al_load_sample("audio/player_eating_2.wav");
    if (!aEat2){
        printf( "\"Eat2\" audio sample not loaded!\n" );
    }

    //al_destroy_sample(audioCredit);
}

void Audio::play(std::string _name) {
    if(_name == "credit") {
        al_play_sample(aCredit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    if(_name == "pregame") {
        al_play_sample(aPregame, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    if(_name == "eat") {
        // Boolean toggle
        playFirst = !playFirst;
        if(playFirst) al_play_sample(aEat1, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        else al_play_sample(aEat2, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}
