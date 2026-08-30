#include <jni.h>
#include "ps2_engine.hpp"
#include "vfs_engine.hpp"
#include <string>
#include <iostream>

extern void start_ps2_native_game();

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_smartport_ps2engine_MainActivity_startEngineNative(JNIEnv* env, jobject) {
    return env->NewStringUTF("PS2 Native Porter & OBB Engine: Ready");
}

JNIEXPORT jboolean JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeMountObb(JNIEnv* env, jobject, jstring obbPath) {
    const char* path = env->GetStringUTFChars(obbPath, nullptr);
    bool success = VfsEngine::getInstance().mountObbDirectory(std::string(path));
    env->ReleaseStringUTFChars(obbPath, path);
    return success ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeStartGameWithObb(JNIEnv* env, jobject) {
    std::cout << "[Native Port] Стартиране на играта с монтиран OBB пакет...\n";
    start_ps2_native_game();
    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeSendInput(JNIEnv* env, jobject, jint buttonsMask) {
    PS2Engine::getInstance().setInputState(static_cast<uint32_t>(buttonsMask));
}

JNIEXPORT void JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeSendAxes(JNIEnv* env, jobject, jfloat lx, jfloat ly, jfloat rx, jfloat ry) {
}

}
