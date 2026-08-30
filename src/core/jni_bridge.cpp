#include <jni.h>
#include "ps2_engine.hpp"
#include "vfs_engine.hpp"
#include <string>
#include <iostream>

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_smartport_ps2engine_MainActivity_startEngineNative(JNIEnv* env, jobject) {
    return env->NewStringUTF("PS2 Native Porter & OBB Engine: Active");
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
    VfsEngine::getInstance().runNativeLoop();
    PS2Engine::getInstance().loadIso("OBB_MOUNTED_GAME");
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
