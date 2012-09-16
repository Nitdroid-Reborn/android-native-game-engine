#ifdef ANDROID
#include <jni.h>
#include <errno.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/window.h>
#include <android/native_window.h>

#include <stdlib.h>
#include <Input/Input.h>

#include <Core/AndroidEngine.h>
#include <Utils/Utils.h>

#include <MemoryManagement/StackAllocator.h>
#include <unistd.h>

#include "client/linux/handler/exception_handler.h"
#include "client/linux/handler/minidump_descriptor.h"

static U64 keyMapper[110];
static android_app* globalApp=0;
U64 lastTime;

bool DumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
                  void* context,
                  bool succeeded) {
  Logger::Log("Dump path: %s\n %p", descriptor.path(), globalApp);

  JavaVM* lJavaVM = globalApp->activity->vm;
  JNIEnv* lJNIEnv = globalApp->activity->env;

  JavaVMAttachArgs lJavaVMAttachArgs;
  lJavaVMAttachArgs.version = JNI_VERSION_1_6;
  lJavaVMAttachArgs.name = "NativeThread";
  lJavaVMAttachArgs.group = NULL;

  jint lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
  if (lResult == JNI_ERR) {
	  Logger::Log("error");
  }

  jobject lNativeActivity = globalApp->activity->clazz;

  jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

  jmethodID method = lJNIEnv->GetMethodID(ClassNativeActivity, "SendCrashReport", "(Ljava/lang/String;)V");

  jstring arg = lJNIEnv->NewStringUTF(descriptor.path());

  lJNIEnv->CallVoidMethod(lNativeActivity, method, arg);
  lJNIEnv->DeleteLocalRef(arg);

  lJavaVM->DetachCurrentThread();

  return succeeded;
}

void Crash() {
  volatile int* a = reinterpret_cast<volatile int*>(NULL);
  *a = 1;
}

I32 mainWindowWidth=0;
I32 mainWindowHeight=0;

ASensorManager* sensorManager;
const ASensor* accelerometerSensor;
ASensorEventQueue* sensorEventQueue;

static void initKeyMapper() {

    for(int i=0;i<110;i++) {
        keyMapper[i]=ENGINE_KEYCODE_UNKNOWN;
    }

    keyMapper[AKEYCODE_0] = ENGINE_KEYCODE_0;
    keyMapper[AKEYCODE_1] = ENGINE_KEYCODE_1;
    keyMapper[AKEYCODE_2] = ENGINE_KEYCODE_2;
    keyMapper[AKEYCODE_3] = ENGINE_KEYCODE_3;
    keyMapper[AKEYCODE_4] = ENGINE_KEYCODE_4;
    keyMapper[AKEYCODE_5] = ENGINE_KEYCODE_5;
    keyMapper[AKEYCODE_6] = ENGINE_KEYCODE_6;
    keyMapper[AKEYCODE_7] = ENGINE_KEYCODE_7;
    keyMapper[AKEYCODE_8] = ENGINE_KEYCODE_8;
    keyMapper[AKEYCODE_9] = ENGINE_KEYCODE_9;
    keyMapper[AKEYCODE_DPAD_UP] = ENGINE_KEYCODE_UP;
    keyMapper[AKEYCODE_DPAD_DOWN] = ENGINE_KEYCODE_DOWN;
    keyMapper[AKEYCODE_DPAD_LEFT] = ENGINE_KEYCODE_LEFT;
    keyMapper[AKEYCODE_DPAD_RIGHT] = ENGINE_KEYCODE_RIGHT;
    keyMapper[AKEYCODE_DPAD_CENTER] = ENGINE_KEYCODE_CENTER;
    keyMapper[AKEYCODE_A] = ENGINE_KEYCODE_A;
    keyMapper[AKEYCODE_B] = ENGINE_KEYCODE_B;
    keyMapper[AKEYCODE_C] = ENGINE_KEYCODE_C;
    keyMapper[AKEYCODE_D] = ENGINE_KEYCODE_D;
    keyMapper[AKEYCODE_E] = ENGINE_KEYCODE_E;
    keyMapper[AKEYCODE_F] = ENGINE_KEYCODE_F;
    keyMapper[AKEYCODE_G] = ENGINE_KEYCODE_G;
    keyMapper[AKEYCODE_H] = ENGINE_KEYCODE_H;
    keyMapper[AKEYCODE_I] = ENGINE_KEYCODE_I;
    keyMapper[AKEYCODE_J] = ENGINE_KEYCODE_J;
    keyMapper[AKEYCODE_K] = ENGINE_KEYCODE_K;
    keyMapper[AKEYCODE_L] = ENGINE_KEYCODE_L;
    keyMapper[AKEYCODE_M] = ENGINE_KEYCODE_M;
    keyMapper[AKEYCODE_N] = ENGINE_KEYCODE_N;
    keyMapper[AKEYCODE_O] = ENGINE_KEYCODE_O;
    keyMapper[AKEYCODE_P] = ENGINE_KEYCODE_P;
    keyMapper[AKEYCODE_Q] = ENGINE_KEYCODE_Q;
    keyMapper[AKEYCODE_R] = ENGINE_KEYCODE_R;
    keyMapper[AKEYCODE_S] = ENGINE_KEYCODE_S;
    keyMapper[AKEYCODE_T] = ENGINE_KEYCODE_T;
    keyMapper[AKEYCODE_U] = ENGINE_KEYCODE_U;
    keyMapper[AKEYCODE_V] = ENGINE_KEYCODE_V;
    keyMapper[AKEYCODE_W] = ENGINE_KEYCODE_W;
    keyMapper[AKEYCODE_X] = ENGINE_KEYCODE_X;
    keyMapper[AKEYCODE_Y] = ENGINE_KEYCODE_Y;
    keyMapper[AKEYCODE_Z] = ENGINE_KEYCODE_Z;
    keyMapper[AKEYCODE_COMMA] = ENGINE_KEYCODE_COMMA;
    keyMapper[AKEYCODE_PERIOD] = ENGINE_KEYCODE_PERIOD;
    keyMapper[AKEYCODE_TAB] = ENGINE_KEYCODE_TAB;
    keyMapper[AKEYCODE_SPACE] = ENGINE_KEYCODE_SPACE;
    keyMapper[AKEYCODE_ENTER] = ENGINE_KEYCODE_ENTER;
    keyMapper[AKEYCODE_DEL] = ENGINE_KEYCODE_DEL;
    keyMapper[AKEYCODE_MINUS] = ENGINE_KEYCODE_MINUS;
    keyMapper[AKEYCODE_EQUALS] = ENGINE_KEYCODE_EQUALS;
    keyMapper[AKEYCODE_LEFT_BRACKET] = ENGINE_KEYCODE_LEFT_BRACKET;
    keyMapper[AKEYCODE_RIGHT_BRACKET] = ENGINE_KEYCODE_RIGHT_BRACKET;
    keyMapper[AKEYCODE_BACKSLASH] = ENGINE_KEYCODE_BACKSLASH;
    keyMapper[AKEYCODE_SEMICOLON] = ENGINE_KEYCODE_SEMICOLON;
    keyMapper[AKEYCODE_APOSTROPHE] = ENGINE_KEYCODE_APOSTROPHE;
    keyMapper[AKEYCODE_SLASH] = ENGINE_KEYCODE_SLASH;
    keyMapper[AKEYCODE_AT] = ENGINE_KEYCODE_AT;
    keyMapper[AKEYCODE_PLUS] = ENGINE_KEYCODE_PLUS;
    keyMapper[AKEYCODE_MENU] = ENGINE_KEYCODE_MENU;
    keyMapper[AKEYCODE_BACK] = ENGINE_KEYCODE_ESCAPE;
}



void AcquireWakeLock(android_app* app) {
    JavaVM* lJavaVM = app->activity->vm;
    JNIEnv* lJNIEnv = app->activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;

    jint lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
    if (lResult == JNI_ERR) {

    }

    jobject lNativeActivity = app->activity->clazz;
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    jmethodID method = lJNIEnv->GetMethodID(ClassNativeActivity, "lock", "()V");
    lJNIEnv->CallVoidMethod(lNativeActivity, method);

    lJavaVM->DetachCurrentThread();
}

void ReleaseWakeLock(android_app* app) {
    JavaVM* lJavaVM = app->activity->vm;
    JNIEnv* lJNIEnv = app->activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;

    jint lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
    if (lResult == JNI_ERR) {

    }

    jobject lNativeActivity = app->activity->clazz;
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    jmethodID method = lJNIEnv->GetMethodID(ClassNativeActivity, "unlock", "()V");
    lJNIEnv->CallVoidMethod(lNativeActivity, method);

    lJavaVM->DetachCurrentThread();
}


/*void SendToServer(android_app* app) {


    JavaVM* lJavaVM = app->activity->vm;
    JNIEnv* lJNIEnv = app->activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;

    jint lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
    if (lResult == JNI_ERR) {

    }

    jobject lNativeActivity = app->activity->clazz;
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    jmethodID method = lJNIEnv->GetMethodID(ClassNativeActivity, "SendToServer", "(I)V");
    lJNIEnv->CallVoidMethod(lNativeActivity, method, );

    lJavaVM->DetachCurrentThread();
}*/



/**
 * Input event callback
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    IEngine* engine = (IEngine*)app->userData;

    int32_t eventType = AInputEvent_getType(event);
    U32 height = 0;
    if(app->window)height=ANativeWindow_getHeight(app->window);

    if(eventType == AINPUT_EVENT_TYPE_MOTION) {
        if(AInputEvent_getSource(event) == AINPUT_SOURCE_TOUCHSCREEN) {

            int32_t action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
            int32_t pointerIndex = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)>>AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

            int32_t pointerId = AMotionEvent_getPointerId(event, pointerIndex);
            TouchEvent engineTouchEvent;
            switch(action) {
                case AMOTION_EVENT_ACTION_DOWN:
                case AMOTION_EVENT_ACTION_POINTER_DOWN:

                    engineTouchEvent.pointerId = pointerId;
                    engineTouchEvent.action = ENGINE_TOUCHACTION_DOWN;
                    engineTouchEvent.posX = AMotionEvent_getX(event, pointerIndex);
                    engineTouchEvent.posY = height - AMotionEvent_getY(event, pointerIndex);


                    engine->ProcessTouchInput(engineTouchEvent);
                    break;

                case AMOTION_EVENT_ACTION_UP:
                case AMOTION_EVENT_ACTION_POINTER_UP:

                    engineTouchEvent.pointerId = pointerId;
                    engineTouchEvent.action = ENGINE_TOUCHACTION_UP;
                    engineTouchEvent.posX = AMotionEvent_getX(event, pointerIndex);
                    engineTouchEvent.posY = height - AMotionEvent_getY(event, pointerIndex);

                    engine->ProcessTouchInput(engineTouchEvent);
                    break;

                case AMOTION_EVENT_ACTION_MOVE:
                    int pointerCount = AMotionEvent_getPointerCount(event);
                    for(int i=0; i<pointerCount; i++) {
                        pointerIndex = i;
                        pointerId = AMotionEvent_getPointerId(event, pointerIndex);

                        engineTouchEvent.pointerId = pointerId;
                        engineTouchEvent.action = ENGINE_TOUCHACTION_MOVE;
                        engineTouchEvent.posX = AMotionEvent_getX(event, pointerIndex);
                        engineTouchEvent.posY = height - AMotionEvent_getY(event, pointerIndex);

                        engine->ProcessTouchInput(engineTouchEvent);
                    }
                    break;
            }
            return 1;
        }
        /*else  {
            KeyEvent engineKeyEvent;
            char action = AMotionEvent_getAction(event);
            if(action==AMOTION_EVENT_ACTION_DOWN) {
                engineKeyEvent.action = ENGINE_KEYACTION_DOWN;
                engineKeyEvent.keyCode = ENGINE_KEYCODE_CENTER;
            }
            else if(action == AMOTION_EVENT_ACTION_UP) {
                engineKeyEvent.action = ENGINE_KEYACTION_UP;
                engineKeyEvent.keyCode = ENGINE_KEYCODE_CENTER;
            }
            else if(action == AMOTION_EVENT_ACTION_MOVE) {
                float x, y;
                x = AMotionEvent_getX(event, 0);
                y = AMotionEvent_getY(event, 0);

                engineKeyEvent.action = ENGINE_KEYACTION_DOWN;

                if(x>0) {
                    engineKeyEvent.keyCode = ENGINE_KEYCODE_RIGHT;
                    touchPadRight=true;
                }
                else if(x<0) {
                    engineKeyEvent.keyCode = ENGINE_KEYCODE_LEFT;
                    touchPadLeft=true;
                }
                else if(y>0) {
                    engineKeyEvent.keyCode = ENGINE_KEYCODE_DOWN;
                    touchPadDown=true;
                }
                else if(y<0) {
                    engineKeyEvent.keyCode = ENGINE_KEYCODE_UP;
                    touchPadUp=true;
                }
            }

            engine->ProcessKeyInput(engineKeyEvent);
            return 1;
        }*/


    }
    else if(eventType == AINPUT_EVENT_TYPE_KEY) {
        EngineKeyCode engineKeyCode = keyMapper[AKeyEvent_getKeyCode(event)];
        char keyAction = AKeyEvent_getAction(event);

        if(engineKeyCode != ENGINE_KEYCODE_UNKNOWN && keyAction != AKEY_EVENT_ACTION_MULTIPLE) {
            KeyEvent engineKeyEvent;
            engineKeyEvent.keyCode = engineKeyCode;
            if(keyAction == AKEY_EVENT_ACTION_DOWN) {
                engineKeyEvent.action = ENGINE_KEYACTION_DOWN;
                Crash();
            }
            else
                engineKeyEvent.action = ENGINE_KEYACTION_UP;


            engine->ProcessKeyInput(engineKeyEvent);



            return 1;
        }
    }

    return 0;
}

/**
 * System event callback
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    IEngine* engine = (IEngine*)app->userData;

    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            engine->OnSaveState();
            break;
        case APP_CMD_INIT_WINDOW:
            engine->OnInitWindow();

            break;


        case APP_CMD_TERM_WINDOW:
            engine->OnTerminateWindow();
            break;


        case APP_CMD_GAINED_FOCUS:

            if (accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(sensorEventQueue,
                        accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(sensorEventQueue,
                        accelerometerSensor, (1000L/60)*1000);
            }
            AcquireWakeLock(app);
            engine->OnGainedFocus();
            break;


        case APP_CMD_LOST_FOCUS:
            engine->OnLostFocus();
            if (accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(sensorEventQueue,
                        accelerometerSensor);
            }
            ReleaseWakeLock(app);
            break;


        case APP_CMD_RESUME:
            engine->OnResume();
            break;

        case APP_CMD_PAUSE:
            engine->OnPause();
            break;
    }
}




/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* app) {

    // Make sure glue isn't stripped.
    app_dummy();

    globalApp = app;

    google_breakpad::MinidumpDescriptor descriptor("/sdcard/");
      google_breakpad::ExceptionHandler eh(descriptor, NULL, DumpCallback,
                                           NULL, true, -1);

    initKeyMapper();

    AndroidEngine engine(app);

    app->userData = &engine;
    //set callbacks
    app->onAppCmd = engine_handle_cmd;
    app->onInputEvent = engine_handle_input;


    //set fullscreen
    ANativeActivity_setWindowFlags(app->activity, AWINDOW_FLAG_FULLSCREEN, 0);

    //restore state of application


    sensorManager = ASensorManager_getInstance();
    accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager,
                ASENSOR_TYPE_ACCELEROMETER);
    sensorEventQueue = ASensorManager_createEventQueue(sensorManager,
                app->looper, LOOPER_ID_USER, NULL, NULL);

    engine.Initialize();


    lastTime = GetCurrentTimeInMsec();
    //engine.Start();
    engine.renderer->Start();
    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.IsRunning() ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(app, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                ASensorEvent event;
                while (ASensorEventQueue_getEvents(sensorEventQueue,
                        &event, 1) > 0) {
                    engine.ProcessAccelerometerInput(event.acceleration.x,
                                                     event.acceleration.y,
                                                     event.acceleration.z);
                }
            }

            // Check if we are exiting.
            if (app->destroyRequested != 0) {
                engine.Release();
                return;
            }
        }
        if(engine.IsQuiting())
            ANativeActivity_finish(app->activity);

        if(engine.IsRunning()) {

            engine.SingleFrame();
        }
    }
}
#endif
//END_INCLUDE(all)
