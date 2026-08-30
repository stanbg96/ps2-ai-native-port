#pragma once
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <iostream>

class RealRenderer {
private:
    ANativeWindow* window = nullptr;
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    bool isReady = false;

public:
    static RealRenderer& getInstance();
    void setWindow(ANativeWindow* win);
    void renderFrame(float r, float g, float b);
    void cleanup();
};
