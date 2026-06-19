#pragma once
#include <3ds.h>
#include <tremor/ivorbisfile.h>

class Sound {
private:
    static const int BUFFER_SIZE = 32 * 1024;
    
    static const int BGM_COUNT = 4;
    static const char* bgm_files[BGM_COUNT];
    OggVorbis_File oggFile;
    u8* stream_buffers[2];
    ndspWaveBuf wave_bufs[2];
    int current_buffer;
    bool is_ogg_playing;

    static const int SE_CHANNEL = 1; 

    struct SEData {
        u8* buffer;
        u32 size;
        int rate;
    };

    SEData se_back;
    SEData se_focus;
    SEData se_press;
    SEData se_scroll;

    int fillOggBuffer(u8* buffer);
    void loadSE(const char* path, SEData& se);
    void playSE(SEData& se, float pitch_multiplier = 1.0f); 
    void freeSE(SEData& se);

public:
    Sound();
    void init();
    void playBGM(const char* path);
    void playBGMRandom();
    void update();
    void exit();

    void playBack();
    void playFocus();
    void playPress();
    void playScroll();
};