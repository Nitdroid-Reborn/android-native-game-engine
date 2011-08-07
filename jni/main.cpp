
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/window.h>
#include <stdlib.h>
#include <stdio.h>

#include "Engine.h"

/**
 * Input event callback
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    Engine* engine = (Engine*)app->userData;
    int32_t eventType = AInputEvent_getType(event);

    if(eventType == AINPUT_EVENT_TYPE_MOTION) {
        return  engine->ProcessTouchInput(event);
    }
    else if(eventType == AINPUT_EVENT_TYPE_KEY) {
        return engine->ProcessKeyInput(event);
    }

    return 0;
}

/**
 * System event callback
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    Engine* engine = (Engine*)app->userData;

    engine->ProcessSystemCommands(cmd);
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* app) {
    Engine engine;

    // Make sure glue isn't stripped.
    app_dummy();

    app->userData = &engine;
    //set callbacks
    app->onAppCmd = engine_handle_cmd;
    app->onInputEvent = engine_handle_input;
    engine.app = app;


    //set fullscreen
    ANativeActivity_setWindowFlags(app->activity, AWINDOW_FLAG_FULLSCREEN, 0);

    //restore state of application
    if (app->savedState != NULL) {
        engine.state = *(struct saved_state*)app->savedState;
    }
    engine.Initialize();


    FILE* pf = fopen("/sdcard/test.txt", "wt");

    if(pf != 0) {
        char* tekst = "abcd";
        fwrite(tekst, sizeof(char), 5, pf);

        fclose(pf);
    }

    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(app, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                engine.ProcessAccelerometer();
            }

            // Check if we are exiting.
            if (app->destroyRequested != 0) {
                engine.TerminateDisplay();
                return;
            }
        }

        if (engine.animating) {
            // Done with events; draw next animation frame.

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine.DrawFrame();
        }
    }
}
//END_INCLUDE(all)
