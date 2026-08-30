#pragma once
#include <cstdint>
#include <vector>
#include <iostream>

struct VoiceChannel {
    bool active;
    uint32_t sampleRate;
    float volumeLeft;
    float volumeRight;
};

class Spu2Engine {
private:
    VoiceChannel voices[24]; // 24 хардуерни гласа на PS2
    std::vector<int16_t> audioBuffer;

public:
    Spu2Engine();
    void initAudioStream(int sampleRate = 48000);
    void decodeAdpcmPacket(const uint8_t* data, size_t size);
    void mixAudio(int16_t* output, size_t samplesCount);
};
