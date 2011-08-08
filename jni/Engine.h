#ifndef ENGINE_H
#define ENGINE_H

#include <android/sensor.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include "FileIO/FileIO.h"
#include "ContentManager/PNGLoader.h"
#include "Input/Input.h"
#include <android_native_app_glue.h>
#include <android/log.h>

#include <time.h>


#include "AudioSystem.h"



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



    AudioSystem audioSystem;
    FileIO* fileIOSystem;

    PNGLoader pngLoader;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;

    struct saved_state state;

    void Initialize();
    void Release();
    int InitDisplay();
    void DrawFrame();
    void TerminateDisplay();

    void ProcessTouchInput(const TouchEvent& event);
    void ProcessKeyInput(const KeyEvent& event);
    void ProcessAccelerometer(float x, float y, float z);

    void onSaveState();
    void onInitWindow();
    void onTerminateWindow();
    void onGainedFocus();
    void onLostFocus();
    void onPause();
    void onResume();

    GLuint texture;




    int frameCounter;
    U64 lastTime;
    float posX, posY;
    float deltaX, deltaY;


    float touchX[4];
    float touchY[4];
    bool touch[4];

};

#endif // ENGINE_H
