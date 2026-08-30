#pragma once
#include <cstdint>
#include <vector>

struct Vertex3D {
    float x, y, z;
    float r, g, b, a;
    float u, v;
};

class GraphicsSynthesizer {
public:
    static void processGifPacket(const uint64_t* data, size_t qword_count, std::vector<Vertex3D>& out_vertices);
};
