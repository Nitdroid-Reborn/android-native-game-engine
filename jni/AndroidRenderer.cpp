#include "AndroidRenderer.h"
#include "Utils.h"
#include <GLES/gl.h>

AndroidRenderer::AndroidRenderer(android_app* app) : IRenderer()
{
    contextValid = false;
    this->app = app;
    initWindow = false;
    terminateWindow = false;
    closing = false;
    active = true;
}

void AndroidRenderer::OnInitWindow() {
    mutex.Lock();

    LOGI("Initialize window");
    initWindow = true;
    mutex.Unlock();
}

void AndroidRenderer::OnTerminateWindow() {
    mutex.Lock();
    terminateWindow = true;
    contextValid = false;
    mutex.Unlock();
}

void AndroidRenderer::OnGainedFocus() {
    mutex.Lock();
    //active = true;
  //  pauseConditionalVariable.Signal();
    mutex.Unlock();
}

void AndroidRenderer::OnLostFocus() {
    mutex.Lock();
    //active = false;
    mutex.Unlock();
}

void AndroidRenderer::InitWindow() {
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, format;
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
        LOGE("Unable to eglMakeCurrent");
        return;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    glEnable(GL_TEXTURE_2D);

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
    //gluPerspective(54.0f, (float)w/(float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, (int) w, (int) h);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    contextValid=true;
}

void AndroidRenderer::TerminateWindow() {
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
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}

void AndroidRenderer::Initialize() {

}

void AndroidRenderer::Release() {
    mutex.Lock();

    closing = true;

    LOGI("Renderer released");

    mutex.Unlock();
}

void AndroidRenderer::Run() {
    U64 currentTime;
    U64 lastTime;

    lastTime = getCurrentTimeInMsec();
    while(1) {
         mutex.Lock();

            if(closing) {
                TerminateWindow();
                LOGI("Renderer stopped");
                mutex.Unlock();
                return;
            }

            if(initWindow) {
                InitWindow();
                initWindow=false;
            }

            if(terminateWindow) {
                TerminateWindow();
                terminateWindow = false;
            }

           // if(active) {
               // mutex.Unlock();
               // pauseConditionalVariable.Wait();


                    currentTime = getCurrentTimeInMsec();
                    float dt = (float)(currentTime - lastTime);
                    fpsClock.update(dt);

                    if(contextValid) {
                       // glClearColor((float)frameCounter/60,1,0,1);
                        glClear(GL_COLOR_BUFFER_BIT);

                        eglSwapBuffers(display, surface);

                         frameCounter++;

                         if(frameCounter>60) {
                             frameCounter=0;
                             LOGI("FPS: %f", 60.0f/((float)fpsClock.getMSeconds()/1000.0f));
                             fpsClock.reset();
                        }

                            lastTime = currentTime;
                    }
            //}

             mutex.Unlock();
        }
   // }
}
