#include "Engine.h"
#include "FileIO/AndroidFileIO.h"
#include <android/keycodes.h>
#include "Utils.h"


static const char hello[] =
                #include "hello_clip.h"
                ;
static const char android[] =
                #include "android_clip.h"
                ;

Engine::Engine()
{
    LOGI("Engine initialized");
    frameCounter = 0;

    posX=250;
    posY=250;
    deltaX=1;
    deltaY=1;
    lastTime = getCurrentTimeInMsec();
}

Engine::~Engine() {
    LOGI("Engine closed");
}


void Engine::Initialize() {



    audioSystem.CreateEngine();
    fileIOSystem = new AndroidFileIO();
    fileIOSystem->Initialize(app->activity->assetManager);

 //   audioSystem.CreateAssetPlayer(app->activity->assetManager, "sound.mp3");
  //  audioSystem.SetAssetPlayerStatus(true);

}

void Engine::Release() {
    audioSystem.Shutdown();
    fileIOSystem->Release();
    delete fileIOSystem;
    fileIOSystem = NULL;
    TerminateDisplay();
}




int Engine::InitDisplay() {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;


    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    this->width = w;
    this->height = h;
    this->state.angle = 0;

    glEnable(GL_TEXTURE_2D);
    texture = pngLoader.load("logo2.png");

    // Initialize GL state.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0.0, w, h, 0.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, (int) w, (int) h);

    return 0;
}

void Engine::DrawFrame() {
    if(display == NULL) {
        return;
    }

    // Just fill the screen with a color.
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    int w = 80;

    float texCoords[8] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);


    for(int i=0;i<4;i++) {
        if(touch[i]==false)
            continue;

        float ver[8] = {
            touchX[i]-w, touchY[i]+w,
            touchX[i]+w, touchY[i]+w,
            touchX[i]-w, touchY[i]-w,
            touchX[i]+w, touchY[i]-w
          };


        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(2, GL_FLOAT, 0, ver);
        glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

    }

    glDisable(GL_TEXTURE_2D);


    posX+=deltaX;
    posY+=deltaY;

    if(posX>=width || posX <=0)deltaX*=-1;
    if(posY>=height || posY <=0)deltaY*=-1;

    eglSwapBuffers(display, surface);

    frameCounter++;

    if(frameCounter>60) {
        frameCounter=0;
        U64 now = getCurrentTimeInMsec();

        float deltaTime = (now - lastTime)/1000.0f;

        LOGI("%f", 60.0f/deltaTime);
        lastTime = getCurrentTimeInMsec();
    }
}

void Engine::TerminateDisplay() {
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }
    animating = 0;
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}

void Engine::ProcessTouchInput(const TouchEvent& event) {
    if(event.action==ENGINE_TOUCHACTION_DOWN) {
     //   LOGI("Touch event: down, id %d posx %f, posy %f", event.pointerId, event.posX, event.posY);
        touch[event.pointerId] = true;
        touchX[event.pointerId] = event.posX;
        touchY[event.pointerId] = event.posY;
    }
    else if(event.action == ENGINE_TOUCHACTION_UP) {
       // LOGI("Touch event: up, id %d posx %f, posy %f", event.pointerId, event.posX, event.posY);
        touch[event.pointerId] = false;
        touchX[event.pointerId] = event.posX;
        touchY[event.pointerId] = event.posY;
    }
    else {
       // LOGI("Touch event: move, id %d posx %f, posy %f", event.pointerId, event.posX, event.posY);
        touchX[event.pointerId] = event.posX;
        touchY[event.pointerId] = event.posY;
    }


}

void Engine::ProcessKeyInput(const KeyEvent& event) {
    if(event.action==ENGINE_KEYACTION_DOWN)
        LOGI("Touch event: down, code %d", event.keyCode);
    else if(event.action == ENGINE_KEYACTION_UP)
        LOGI("Touch event: up, code %d", event.keyCode);

}




void Engine::onGainedFocus() {
    animating = 1;
}

void Engine::onLostFocus() {
    animating = 0;
}

void Engine::onSaveState() {
    app->savedState = malloc(sizeof(struct saved_state));
    *((struct saved_state*)app->savedState) = state;
    app->savedStateSize = sizeof(struct saved_state);
}

void Engine::onInitWindow() {
    if (app->window != NULL) {
        InitDisplay();
        DrawFrame();
    }
}

void Engine::onTerminateWindow() {
    TerminateDisplay();
}

void Engine::onPause() {

}

void Engine::onResume() {

}


void Engine::ProcessAccelerometer(float x, float y, float z) {
           /* LOGI("accelerometer: x=%f y=%f z=%f",
                    x, y, z);*/
}
