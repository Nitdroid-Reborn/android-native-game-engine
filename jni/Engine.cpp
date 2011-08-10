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

#define PI 3.1415926535897932f

static void gluPerspective(GLfloat fovy, GLfloat aspect,
                           GLfloat zNear, GLfloat zFar)
{
    GLfloat xmin, xmax, ymin, ymax;

    ymax = zNear * (GLfloat)tan(fovy * PI / 360);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustumx((GLfixed)(xmin * 65536), (GLfixed)(xmax * 65536),
               (GLfixed)(ymin * 65536), (GLfixed)(ymax * 65536),
               (GLfixed)(zNear * 65536), (GLfixed)(zFar * 65536));
}



Engine::Engine()
{
    LOGI("Engine initialized");
    frameCounter = 0;

    posX=250;
    posY=250;
    deltaX=1;
    deltaY=1;
    lastTime = getCurrentTimeInMsec();
    exit = false;


    centerKey = new VirtualSingleKey(ENGINE_KEYCODE_CENTER, 500, 200, 50);
    dpad = new VirtualDPad(125, 355, 100, 25);
}

Engine::~Engine() {
    delete dpad;
    delete centerKey;
    LOGI("Engine closed");
}


void Engine::Initialize() {



    audioSystem.CreateEngine();
    fileIOSystem = new AndroidFileIO();
    fileIOSystem->Initialize(app->activity->assetManager);

    inputSystem = new Input();

    virtualInputSystem = new VirtualInput();
    virtualInputSystem->AddKey(centerKey);
    virtualInputSystem->AddKey(dpad);


    model.Load("model.ms3d");

 //   audioSystem.CreateAssetPlayer(app->activity->assetManager, "sound.mp3");
  //  audioSystem.SetAssetPlayerStatus(true);

}

void Engine::Release() {
    audioSystem.Shutdown();
    fileIOSystem->Release();
    delete fileIOSystem;
    fileIOSystem = NULL;

    delete inputSystem;
    inputSystem = NULL;

    delete virtualInputSystem;
    virtualInputSystem = NULL;
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
    //glOrthof(0.0, w, h, 0.0, 0.0, 1.0);
    gluPerspective(54.0f, (float)w/(float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, (int) w, (int) h);

    return 0;
}

void Engine::Render() {
    if(display == NULL) {
        return;
    }

    // Just fill the screen with a color.
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    int w = 80;

    float texCoords[8] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glColor4f(1,1,1,1);
    glPushMatrix();
        glScalef(0.01, 0.01, 0.01);
        glTranslatef(0, -20, -80);
        glRotatef(angleY, 0, 1, 0);
        glRotatef(angleX, 1, 0, 0);

        model.Render();

        glPopMatrix();

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrthof(0.0, width, height, 0.0, 0.0, 1.0);
   centerKey->Draw();
   dpad->Draw();

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(54.0f, (float)width/(float)height, 0.1f, 100.0f);
   glMatrixMode(GL_MODELVIEW);

    /*for(int i=0;i<4;i++) {
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

    }*/


    /*GLfloat vertices[] = {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
                          1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
                          1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
                          -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
                          -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
                          1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5


    GLubyte indices[] = {0,1,2,3,
                         4,5,6,7,
                         8,9,10,11,
                         12,13,14,15,
                         16,17,18,19,
                         20,21,22,23};*/


  /*  glPushMatrix();
    glScalef(0.01, 0.01, 0.01);
    glTranslatef(0, -20, -80);
    glRotatef(angle, 0, 1, 0);*/
   /* glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_BYTE, indices);

*/
  /*  model.Render();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    posX+=deltaX;
    posY+=deltaY;

    if(posX>=width || posX <=0)deltaX*=-1;
    if(posY>=height || posY <=0)deltaY*=-1;*/

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
    if(!virtualInputSystem->NewTouchEvent(event))
        inputSystem->ProcessTouchEvent(event);
}

void Engine::ProcessKeyInput(const KeyEvent& event) {
    if(event.keyCode==ENGINE_KEYCODE_ESCAPE && event.action == ENGINE_KEYACTION_DOWN) {
        exit=true;
    }
    else
        inputSystem->ProcessKeyEvent(event);
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
        Render();
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

void Engine::onFrameStart() {
    vector<KeyEvent> events = virtualInputSystem->GetEvents();
    for(int i=0;i<events.size();++i) {
        inputSystem->ProcessKeyEvent(events[i]);
    }
    inputSystem->StartFrame();
}

void Engine::onFrameEnd() {
    inputSystem->EndFrame();
}

void Engine::update() {
    /*if(inputSystem->GetKeyState()->IsKeyPressed(ENGINE_KEYCODE_CENTER))
        LOGI("G pressed");*/

    if(inputSystem->GetKeyState()->IsKeyJustPressed(ENGINE_KEYCODE_CENTER))
        LOGI("G just pressed");

    if(inputSystem->GetKeyState()->IsKeyPressed(ENGINE_KEYCODE_LEFT))
        angleY-=1.0f;
    if(inputSystem->GetKeyState()->IsKeyPressed(ENGINE_KEYCODE_RIGHT))
        angleY+=1.0f;
    if(inputSystem->GetKeyState()->IsKeyPressed(ENGINE_KEYCODE_UP))
        angleX+=1.0f;
    if(inputSystem->GetKeyState()->IsKeyPressed(ENGINE_KEYCODE_DOWN))
        angleX-=1.0f;
}
