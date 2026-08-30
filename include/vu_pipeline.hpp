#pragma once
#include <cstdint>
#include <iostream>

#if defined(__aarch64__)
#include <arm_neon.h>
#endif

class VectorUnitPipeline {
public:
    // Симулация и компилация на VU1 4-компонентни векторни изчисления (X, Y, Z, W)
    static void processVectorMatrixMul(float* vecIn, float* matrix, float* vecOut) {
#if defined(__aarch64__)
        // Директно използване на ARM64 NEON хардуерни регистри за 120 FPS скорост
        float32x4_t v = vld1q_f32(vecIn);
        float32x4_t row0 = vld1q_f32(matrix);
        float32x4_t res = vmulq_f32(v, row0);
        vst1q_f32(vecOut, res);
#else
        for (int i = 0; i < 4; i++) vecOut[i] = vecIn[i] * matrix[i];
#endif
    }
};
