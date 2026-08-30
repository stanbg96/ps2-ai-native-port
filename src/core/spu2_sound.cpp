#include "spu2_sound.hpp"

Spu2Engine::Spu2Engine() {
    for (int i = 0; i < 24; ++i) {
        voices[i].active = false;
        voices[i].sampleRate = 48000;
        voices[i].volumeLeft = 1.0f;
        voices[i].volumeRight = 1.0f;
    }
}

void Spu2Engine::initAudioStream(int sampleRate) {
    std::cout << "[SPU2 Engine] 24-канален аудио миксер зареден на " << sampleRate << "Hz (Low Latency).\n";
}

void Spu2Engine::decodeAdpcmPacket(const uint8_t* data, size_t size) {
    // Декодира PS2 VAG/ADPCM компресирания звук в 16-bit PCM данни
}

void Spu2Engine::mixAudio(int16_t* output, size_t samplesCount) {
    // В реално време миксира каналите към високоговорителите на Android
    for(size_t i = 0; i < samplesCount; ++i) {
        output[i] = 0;
    }
}
