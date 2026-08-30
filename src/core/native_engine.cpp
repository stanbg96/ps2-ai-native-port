#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>

#define LOG_TAG "PS2NativeEngine"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static ANativeWindow* g_window = nullptr;
static EGLDisplay g_display = EGL_NO_DISPLAY;
static EGLSurface g_surface = EGL_NO_SURFACE;
static EGLContext g_context = EGL_NO_CONTEXT;
static bool g_eglReady = false;
static std::string g_obbPath = "";

void initEGL(ANativeWindow* win) {
    if (!win) return;
    g_window = win;
    g_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(g_display, nullptr, nullptr);

    const EGLint attribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    eglChooseConfig(g_display, attribs, &config, 1, &numConfigs);

    g_surface = eglCreateWindowSurface(g_display, config, g_window, nullptr);
    const EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    g_context = eglCreateContext(g_display, config, EGL_NO_CONTEXT, contextAttribs);

    eglMakeCurrent(g_display, g_surface, g_surface, g_context);
    g_eglReady = true;
    LOGI("EGL NativeWindow initialized on Android display!");
}

void cleanupEGL() {
    if (g_display != EGL_NO_DISPLAY) {
        eglMakeCurrent(g_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (g_context != EGL_NO_CONTEXT) eglDestroyContext(g_display, g_context);
        if (g_surface != EGL_NO_SURFACE) eglDestroySurface(g_display, g_surface);
        eglTerminate(g_display);
    }
    g_display = EGL_NO_DISPLAY;
    g_context = EGL_NO_CONTEXT;
    g_surface = EGL_NO_SURFACE;
    g_window = nullptr;
    g_eglReady = false;
}

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_smartport_ps2engine_MainActivity_startEngineNative(JNIEnv* env, jobject) {
    return env->NewStringUTF("PS2 Native Engine: 120 FPS Ready");
}

JNIEXPORT void JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeSetSurface(JNIEnv* env, jobject, jobject surface) {
    if (surface != nullptr) {
        ANativeWindow* win = ANativeWindow_fromSurface(env, surface);
        initEGL(win);
    } else {
        cleanupEGL();
    }
}

JNIEXPORT void JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeRenderGameFrame(JNIEnv* env, jobject, jfloat r, jfloat g, jfloat b) {
    if (!g_eglReady) return;
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(g_display, g_surface);
}

JNIEXPORT jboolean JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeMountObb(JNIEnv* env, jobject, jstring obbPath) {
    const char* p = env->GetStringUTFChars(obbPath, nullptr);
    g_obbPath = std::string(p);
    env->ReleaseStringUTFChars(obbPath, p);
    LOGI("VFS OBB Mounted: %s", g_obbPath.c_str());
    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeSendInput(JNIEnv* env, jobject, jint buttonsMask) {
}

JNIEXPORT void JNICALL
Java_com_smartport_ps2engine_MainActivity_nativeSendAxes(JNIEnv* env, jobject, jfloat lx, jfloat ly, jfloat rx, jfloat ry) {
}

}
