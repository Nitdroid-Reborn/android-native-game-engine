#include "AndroidRenderer.h"
#include <Utils/Utils.h>
#include <GLES/gl.h>
#include "SpriteBatcher.h"
#include <unistd.h>
#include <ContentManager/AndroidContentManager.h>
#include "TextBox.h"
#include "Font.h"
#include <Utils/Profiler.h>

IRenderer* IRenderer::singleton = NULL;

ProfilerManager rendererProfileManager;
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
    Log(1, "Renderer window initialized");
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

    const AndroidContentManager* manager = (const AndroidContentManager*)IContentManager::get();

    context = eglCreateContext(display, config, manager->GetEGLContext(), NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        Log("Unable to eglMakeCurrent");
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


    textBox.SetSize(0, w, 0, h);

    contextValid=true;
}

void AndroidRenderer::TerminateWindow() {
    Log(1, "Renderer window destroyed");
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
    ASSERT(!singleton, "FileIO system already initialized");

    singleton = this;

    batcher = new SpriteBatcher(1000);

    TextureInfo info;
    info.format = TextureInfo::LUMINANCE_ALPHA;
    info.width = 64;
    info.height = 128;

    fontTexture = TextureHandle(IContentManager::get()->GetTextureManager()->GetTexture((void*)fontData, info));

    textBox.fontTex = fontTexture;
    fontTexture.Get()->Bind();

    Log(1, "Android Renderer initialized");
}

void AndroidRenderer::Release() {
    mutex.Lock();
    mainLoopCond.Signal();

    usleep(500);
    closing = true;
    delete batcher;
    singleton = NULL;
    mutex.Unlock();
}

void AndroidRenderer::Run() {

   // lastTime = getCurrentTimeInMsec();
    while(1) {
         mutex.Lock();
         mainLoopCond.Signal();


            if(closing) {
                TerminateWindow();
                mutex.Unlock();
                Log(1, "Android Renderer released");
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

            {
                    PROFILE("Render", &rendererProfileManager);
                    currentTime = GetCurrentTimeInMsec();
                    float dt = (float)(currentTime - lastTime);
                    fpsClock.update(dt);

                    //if(contextValid) {
                    glClearColor(0,0,0,1);
                    glClear(GL_COLOR_BUFFER_BIT);

                    {
                       PROFILE("Batch", &rendererProfileManager);
                    TextureRegion r(0, 0, 1, 1);

                   /* if(oldSprites.size() && oldSprites[0].texture) {
                        batcher->BeginBatch(oldSprites[0].texture);
                    }
                    else*/
                    batcher->BeginBatch(fontTexture.Get());
                    for(int i=0;i<oldSprites.size();i++) {

                        {
                   //     PROFILE("Draw Sprite", &rendererProfileManager);

                        batcher->DrawSprite(oldSprites[i].x, oldSprites[i].y,
                                            oldSprites[i].width, oldSprites[i].height,
                                            oldSprites[i].texRegion, oldSprites[i].angle);
                        }

                    }
                    batcher->EndBatch();
                    }

                    {
                      PROFILE("Swap buffers", &rendererProfileManager);
                    if(contextValid)
                        eglSwapBuffers(display, surface);
                    }



                     frameCounter++;

                     if(frameCounter>60) {
                         frameCounter=0;
                         Log(0, "FPS: %f", 60.0f/((float)fpsClock.getMSeconds()/1000.0f));
                         fpsClock.reset();
                    }

                    lastTime = currentTime;
                   // }
            //}
            }
            rendererProfileManager.DumpProfileDataToBuffer();
             mutex.Unlock();
      //  }


    }
}

void AndroidRenderer::Wait() {
    mutex.Lock();
    oldSprites = sprites;
    sprites.clear();
    mutex.Unlock();
    mainLoopCond.Wait();
    if(ProfilerManager::profilerEnabled)
        IRenderer::get()->DrawString(5, 200, rendererProfileManager.outputBuffer.Get());
}

void AndroidRenderer::DrawSprite(F32 x, F32 y, F32 width, F32 height, F32 angle) {
    Sprite s;;
    s.x = x;
    s.y = y;
    s.width = width;
    s.height = height;
    s.angle = angle;
    s.texture = NULL;

    sprites.push_back(s);
}

void AndroidRenderer::DrawSprite(F32 x, F32 y, F32 width, F32 height,
                                 TextureRegion &region, TextureHandle& handle, F32 angle) {
    Sprite s;;
    s.x = x;
    s.y = y;
    s.width = width;
    s.height = height;
    s.angle = angle;
    s.texRegion = region;
    s.texture = handle.Get();

    sprites.push_back(s);
}

void AndroidRenderer::DrawString(int x, int y, char * str) {
    textBox.DrawStr(x, y, str);
}

