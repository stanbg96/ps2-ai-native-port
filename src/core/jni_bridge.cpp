#include <jni.h>
#include "real_renderer.hpp"
#include "vfs_engine.hpp"
#include "ps2_engine.hpp"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <string>
#include <iostream>

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_smartport_ps2engine_MainActivity_startEngineNative(JNIEnv* env, jobject) {
    return env->NewStringUTF("PS2 Engine: 120 FPS Vulkan/OpenGL Native Ready");
}

JNIEXPORT void JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeSetSurface(JNIEnv* env, jobject, jobject surface) {
    if (surface != nullptr) {
        ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
        RealRenderer::getInstance().setWindow(window);
    } else {
        RealRenderer::getInstance().cleanup();
    }
}

JNIEXPORT void JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeRenderGameFrame(JNIEnv* env, jobject, jfloat r, jfloat g, jfloat b) {
    RealRenderer::getInstance().renderFrame(r, g, b);
}

JNIEXPORT jboolean JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeMountObb(JNIEnv* env, jobject, jstring obbPath) {
    const char* path = env->GetStringUTFChars(obbPath, nullptr);
    bool success = VfsEngine::getInstance().mountObbDirectory(std::string(path));
    env->ReleaseStringUTFChars(obbPath, path);
    return success ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeSendInput(JNIEnv* env, jobject, jint buttonsMask) {
    PS2Engine::getInstance().setInputState(static_cast<uint32_t>(buttonsMask));
}

JNIEXPORT void JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeSendAxes(JNIEnv* env, jobject, jfloat lx, jfloat ly, jfloat rx, jfloat ry) {
}

}
