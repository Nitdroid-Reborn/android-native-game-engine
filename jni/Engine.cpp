#include "Engine.h"
#include <android/keycodes.h>
#include "Utils.h"

Engine::Engine()
{
    LOGI("Engine initialized");
    frameCounter = 0;

    posX=250;
    posY=250;
    deltaX=1;
    deltaY=1;
    lastTime = getCurrentTime();
}

Engine::~Engine() {
    LOGI("Engine closed");
}


void Engine::Initialize() {
    sensorManager = ASensorManager_getInstance();
    accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager,
                ASENSOR_TYPE_ACCELEROMETER);
    sensorEventQueue = ASensorManager_createEventQueue(sensorManager,
                app->looper, LOOPER_ID_USER, NULL, NULL);
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

    for(int i=0;i<4;i++) {
        if(touch[i]==false)
            continue;

        float ver[8] = {
            touchX[i]-w, touchY[i]+w,
            touchX[i]+w, touchY[i]+w,
            touchX[i]-w, touchY[i]-w,
            touchX[i]+w, touchY[i]-w
          };

        glColor4f(1,0,0,1);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, ver);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
    }


    posX+=deltaX;
    posY+=deltaY;

    if(posX>=width || posX <=0)deltaX*=-1;
    if(posY>=height || posY <=0)deltaY*=-1;

    eglSwapBuffers(display, surface);

    frameCounter++;

    if(frameCounter>60) {
        frameCounter=0;
        double now = getCurrentTime();

        double deltaTime = now - lastTime;

        LOGI("%f", 60.0f/deltaTime);
        lastTime = getCurrentTime();
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

int32_t Engine::ProcessTouchInput(AInputEvent *event) {
    if(AInputEvent_getSource(event) == AINPUT_SOURCE_TOUCHSCREEN) {



        int32_t action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
        int32_t pointerIndex = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)>>AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

        int32_t pointerId = AMotionEvent_getPointerId(event, pointerIndex);

        switch(action) {
            case AMOTION_EVENT_ACTION_DOWN:
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
                touch[pointerId] = true;
                touchX[pointerId] = AMotionEvent_getX(event, pointerIndex);
                touchY[pointerId] = AMotionEvent_getY(event, pointerIndex);
                break;

            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_POINTER_UP:
                touch[pointerId] = false;
                touchX[pointerId] = AMotionEvent_getX(event, pointerIndex);
                touchY[pointerId] = AMotionEvent_getY(event, pointerIndex);
                break;

            case AMOTION_EVENT_ACTION_MOVE:
                int pointerCount = AMotionEvent_getPointerCount(event);
                for(int i=0; i<pointerCount; i++) {
                    pointerIndex = i;
                    pointerId = AMotionEvent_getPointerId(event, pointerIndex);
                    touchX[pointerId] = AMotionEvent_getX(event, pointerIndex);
                    touchY[pointerId] = AMotionEvent_getY(event, pointerIndex);
                }
                break;
        }
        return 1;
    }
    else  {
        if(AMotionEvent_getAction(event)==AMOTION_EVENT_ACTION_DOWN) {
            LOGI("Touchpad pressed");
        }
        else if(AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_MOVE) {
            float x, y;
            x = AMotionEvent_getX(event, 0);
            y = AMotionEvent_getY(event, 0);

            if(x>0) LOGI("Touchpad right");
            if(x<0) LOGI("Touchpad left");
            if(y>0) LOGI("Touchpad down");
            if(y<0) LOGI("Touchpad up");
        }
    }
    return 0;
}

int32_t Engine::ProcessKeyInput(AInputEvent *event) {
    if(AKeyEvent_getKeyCode(event)==AKEYCODE_BACK)
        return 0;
    LOGI("Key input, %d,  %d", AKeyEvent_getAction(event), AKeyEvent_getKeyCode(event));
}


void Engine::AcquireWakeLock() {
    JavaVM* lJavaVM = app->activity->vm;
    JNIEnv* lJNIEnv = app->activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;

    jint lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
    if (lResult == JNI_ERR) {
        LOGI("error");
    }

    jobject lNativeActivity = app->activity->clazz;
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    jmethodID method = lJNIEnv->GetMethodID(ClassNativeActivity, "lock", "()V");
    lJNIEnv->CallVoidMethod(lNativeActivity, method);

    lJavaVM->DetachCurrentThread();
}

void Engine::ReleaseWakeLock() {
    JavaVM* lJavaVM = app->activity->vm;
    JNIEnv* lJNIEnv = app->activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;

    jint lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
    if (lResult == JNI_ERR) {
        LOGI("error");
    }

    jobject lNativeActivity = app->activity->clazz;
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    jmethodID method = lJNIEnv->GetMethodID(ClassNativeActivity, "unlock", "()V");
    lJNIEnv->CallVoidMethod(lNativeActivity, method);

    lJavaVM->DetachCurrentThread();
}

void Engine::ProcessSystemCommands(int32_t command) {
    switch (command) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)app->savedState) = state;
            app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != NULL) {
                InitDisplay();
                DrawFrame();
            }
            break;


        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            TerminateDisplay();
            break;


        case APP_CMD_GAINED_FOCUS:
        /*    if (accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(sensorEventQueue,
                        accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(sensorEventQueue,
                        accelerometerSensor, (1000L/60)*1000);
            }*/
            animating = 1;
            AcquireWakeLock();
        break;


        case APP_CMD_LOST_FOCUS:
            /*if (accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(sensorEventQueue,
                        accelerometerSensor);
            }*/
            // Also stop animating.*/
            animating = 0;
            DrawFrame();
            ReleaseWakeLock();
        break;


        case APP_CMD_RESUME:

            break;

        case APP_CMD_PAUSE:

            break;
    }
}

void Engine::ProcessAccelerometer() {
    if(accelerometerSensor != NULL) {
        ASensorEvent event;
        while (ASensorEventQueue_getEvents(sensorEventQueue,
                &event, 1) > 0) {
            /*LOGI("accelerometer: x=%f y=%f z=%f",
                    event.acceleration.x, event.acceleration.y,
                    event.acceleration.z);*/
        }
    }
}
