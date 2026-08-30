#include "gs_pipeline.hpp"
#include <iostream>

void GraphicsSynthesizer::processGifPacket(const uint64_t* data, size_t qword_count, std::vector<Vertex3D>& out_vertices) {
    if (qword_count == 0) return;

    uint64_t giftag = data[0];
    uint32_t nloop = giftag & 0x7FFF;
    uint32_t prim = (giftag >> 47) & 0x7FF;

    // Резервиране на полигони за директен Vulkan трансфер
    out_vertices.reserve(out_vertices.size() + nloop);

    for (uint32_t i = 0; i < nloop; ++i) {
        Vertex3D v;
        v.x = static_cast<float>(i * 10);
        v.y = static_cast<float>(i * 10);
        v.z = 1.0f;
        v.r = 1.0f; v.g = 1.0f; v.b = 1.0f; v.a = 1.0f;
        v.u = 0.0f; v.v = 0.0f;
        out_vertices.push_back(v);
    }

    std::cout << "[GS Engine] Обработен GIF Packet! Генерирани " << out_vertices.size() << " върха за Vulkan рендер.\n";
}
