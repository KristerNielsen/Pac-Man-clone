#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <string>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

class Audio {
    public:
        static void init();
        static void play(std::string _name);
};

#endif // AUDIO_H
