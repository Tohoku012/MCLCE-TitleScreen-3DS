#include "Sound.h"
#include <stdlib.h>
#include <string.h>
#include <time.h> 

const char* Sound::bgm_files[] = {
    "romfs:/Media/sound/bgm/menu1.ogg",
    "romfs:/Media/sound/bgm/menu2.ogg",
    "romfs:/Media/sound/bgm/menu3.ogg",
    "romfs:/Media/sound/bgm/menu4.ogg"
};

Sound::Sound() : stream_buffers{NULL, NULL}, current_buffer(0), is_ogg_playing(false) {
    se_back = {NULL, 0, 44100};
    se_focus = {NULL, 0, 44100};
    se_press = {NULL, 0, 44100};
    se_scroll = {NULL, 0, 44100};
}

void Sound::init() {
    srand((unsigned int)svcGetSystemTick());

    ndspInit();
    ndspSetOutputCount(2);

    shuffleBGMOrder();
    
    float mix_bgm[12] = {0};
    mix_bgm[0] = 1.0; mix_bgm[1] = 1.0;
    ndspChnSetMix(0, mix_bgm);

    float mix_se[12] = {0};
    mix_se[0] = 1.0; mix_se[1] = 1.0;
    ndspChnSetMix(SE_CHANNEL, mix_se);

    loadSE("romfs:/Media/sound/menu/back.ogg", se_back);
    loadSE("romfs:/Media/sound/menu/focus.ogg", se_focus);
    loadSE("romfs:/Media/sound/menu/press.ogg", se_press);
    loadSE("romfs:/Media/sound/menu/scroll.ogg", se_scroll);
}

void Sound::loadSE(const char* path, SEData& se) {
    OggVorbis_File vf;
    FILE* f = fopen(path, "rb");
    if (f == NULL) return;

    if (ov_open(f, &vf, NULL, 0) < 0) {
        fclose(f);
        return;
    }

    vorbis_info* info = ov_info(&vf, -1);
    se.rate = info->rate;

    u32 total_samples = (u32)ov_pcm_total(&vf, -1);
    se.size = total_samples * info->channels * 2; // 16bit = 2bytes
    se.buffer = (u8*)linearAlloc(se.size);

    if (se.buffer != NULL) {
        u32 bytes_read = 0;
        int bitstream;
        while (bytes_read < se.size) {
            long ret = ov_read(&vf, (char*)(se.buffer + bytes_read), se.size - bytes_read, &bitstream);
            if (ret <= 0) break;
            bytes_read += ret;
        }
        GSPGPU_FlushDataCache(se.buffer, se.size);
    }
    ov_clear(&vf);
}

void Sound::playSE(SEData& se, float pitch_multiplier) {
    if (se.buffer == NULL || se.size == 0) return;

    ndspChnReset(SE_CHANNEL);
    
    ndspChnSetRate(SE_CHANNEL, (int)(se.rate * pitch_multiplier)); 
    ndspChnSetFormat(SE_CHANNEL, NDSP_FORMAT_STEREO_PCM16);

    static ndspWaveBuf se_buf; 
    memset(&se_buf, 0, sizeof(ndspWaveBuf));
    se_buf.data_vaddr = se.buffer;
    se_buf.nsamples = se.size / 4; 
    se_buf.looping = false;

    ndspChnWaveBufAdd(SE_CHANNEL, &se_buf);
}

void Sound::playBack()   { playSE(se_back); }
void Sound::playFocus() {
    
    float pitch = 0.5f;
    
    playSE(se_focus, pitch);
}
void Sound::playPress()  { playSE(se_press, 0.5f); }
void Sound::playScroll() { playSE(se_scroll); }

int Sound::fillOggBuffer(u8* buffer) {
    int bytes_read = 0;
    int bitstream;
    while (bytes_read < BUFFER_SIZE) {
        long ret = ov_read(&oggFile, (char*)(buffer + bytes_read), BUFFER_SIZE - bytes_read, &bitstream);
        if (ret == 0)  break;
        if (ret < 0)   return -1;
        bytes_read += ret;
    }
    if (bytes_read > 0) GSPGPU_FlushDataCache(buffer, bytes_read);
    return bytes_read;
}

void Sound::playBGM(const char* path) {
    if (is_ogg_playing) {
        ndspChnReset(0);
        ov_clear(&oggFile);
        is_ogg_playing = false;
    }

    FILE* f = fopen(path, "rb");
    if (f == NULL) return;
    if (ov_open(f, &oggFile, NULL, 0) < 0) { fclose(f); return; }

    vorbis_info* info = ov_info(&oggFile, -1);
    ndspChnSetRate(0, info->rate);
    ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);

    if (stream_buffers[0] == NULL) {
        stream_buffers[0] = (u8*)linearAlloc(BUFFER_SIZE);
        stream_buffers[1] = (u8*)linearAlloc(BUFFER_SIZE);
    }

    for (int i = 0; i < 2; i++) {
        int size = fillOggBuffer(stream_buffers[i]);
        if (size <= 0) break;
        memset(&wave_bufs[i], 0, sizeof(ndspWaveBuf));
        wave_bufs[i].data_vaddr = stream_buffers[i];
        wave_bufs[i].nsamples = size / 4;
        ndspChnWaveBufAdd(0, &wave_bufs[i]);
    }
    current_buffer = 0;
    is_ogg_playing = true;
}

void Sound::shuffleBGMOrder() {
    // フィッシャー–イェーツのシャッフル
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = bgm_order[i];
        bgm_order[i] = bgm_order[j];
        bgm_order[j] = temp;
    }
}

void Sound::playBGMRandom() {
    if (current_idx >= 4) {
        shuffleBGMOrder();
        current_idx = 0;
    }

    int next_index = bgm_order[current_idx];
    current_idx++;
    
    playBGM(bgm_files[next_index]);
}

void Sound::update() {
    if (!is_ogg_playing) return;

    if (wave_bufs[current_buffer].status == NDSP_WBUF_DONE) {
        int size = fillOggBuffer(stream_buffers[current_buffer]);
        if (size > 0) {
            wave_bufs[current_buffer].nsamples = size / 4;
            ndspChnWaveBufAdd(0, &wave_bufs[current_buffer]);
            current_buffer = !current_buffer;
        } else {
            playBGMRandom();
        }
    }
}

void Sound::freeSE(SEData& se) {
    if (se.buffer != NULL) linearFree(se.buffer);
}

void Sound::exit() {
    if (is_ogg_playing) {
        ndspChnReset(0);
        ov_clear(&oggFile);
    }
    ndspChnReset(SE_CHANNEL);

    if (stream_buffers[0] != NULL) linearFree(stream_buffers[0]);
    if (stream_buffers[1] != NULL) linearFree(stream_buffers[1]);

    freeSE(se_back);
    freeSE(se_focus);
    freeSE(se_press);
    freeSE(se_scroll);

    ndspExit();
}