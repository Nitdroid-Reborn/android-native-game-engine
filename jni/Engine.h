#ifndef ENGINE_H
#define ENGINE_H

#include <android/sensor.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android_native_app_glue.h>
#include <android/log.h>

#include <time.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};


class Engine
{
public:
    Engine();
    ~Engine();

    android_app * app;
    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;

    struct saved_state state;

    void Initialize();
    int InitDisplay();
    void DrawFrame();
    void TerminateDisplay();

    int32_t ProcessTouchInput(AInputEvent*event);
    int32_t ProcessKeyInput(AInputEvent*event);


    void ProcessSystemCommands(int32_t command);
    void ProcessAccelerometer();

    void AcquireWakeLock();
    void ReleaseWakeLock();


    int frameCounter;
    double lastTime;
    float posX, posY;
    float deltaX, deltaY;


    float touchX[4];
    float touchY[4];
    bool touch[4];

};

#endif // ENGINE_H
