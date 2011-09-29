#include "AndroidRenderer.h"
#include <Utils/Utils.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include "SpriteBatcher.h"
#include <unistd.h>
#include <ContentManager/AndroidContentManager.h>
#include "TextBox.h"
#include "Font.h"
#include <Utils/Profiler.h>
#include <algorithm>

IRenderer* IRenderer::singleton = NULL;

ProfilerManager rendererProfileManager;
static int profileCounter = 0;

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    Logger::Log("GL %s = %s\n", name, v);
}



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
    Logger::Log(1, "Renderer window initialized");
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
        Logger::Log("Unable to eglMakeCurrent");
        return;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);


    Logger::Log("%d", ANativeWindow_getHeight(app->window));

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
    glOrthof(0.0, w, 0.0, h, -1.0, 1.0);
    //gluPerspective(54.0f, (float)w/(float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, (int) w, (int) h);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DITHER);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    contextValid=true;
}

void AndroidRenderer::TerminateWindow() {
    Logger::Log(1, "Renderer window destroyed");
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
    ASSERT(!singleton, "Render system already initialized");

    singleton = this;

    batcher = new SpriteBatcher(1000);

    SpriteBatcher::batcherProfileManager = &rendererProfileManager;

    myFont.Load("/sdcard/Verdana.ttf", 15);


    textBox.font = &myFont;

    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    lua_State* L =  ScriptManager::Get()->getState();

    luabind::module(L)
    [
        luabind::class_<ITexture>("Texture")
    ];

    luabind::module(L)
    [
        luabind::class_<TextureRegion>("TextureRegion")
            .def(luabind::constructor<>())
            .def(luabind::constructor<F32, F32, F32, F32>())
    ];

    luabind::module(L)
    [
        luabind::class_<IRenderer>("Renderer")
            .def("DrawSprite", (void (IRenderer::*)(F32, F32, F32, F32, F32, F32))&IRenderer::DrawSprite)
            .def("DrawSprite", (void (IRenderer::*)(F32, F32, F32, F32, F32, TextureRegion&, TextureHandle&, F32))&IRenderer::DrawSprite)
            .def("DrawString", &IRenderer::DrawString)
            .scope
            [
                luabind::def("Get", IRenderer::get)
            ]
            .enum_("SpriteLayers")
            [
                luabind::value("NORMAL_LAYER", NORMAL_LAYER)
            ]
    ];



    Logger::Log(1, "Android Renderer initialized");
}

void AndroidRenderer::Release() {
    mutex.Lock();
    mainLoopCond.Signal();

    myFont.Dispose();
    usleep(500);
    closing = true;
    delete batcher;
    batcher = NULL;
    delete font;
    font = NULL;
    singleton = NULL;
    mutex.Unlock();
}

void AndroidRenderer::Run() {

   // lastTime = getCurrentTimeInMsec();
    while(1) {
         mutex.Lock();



            if(closing) {
                TerminateWindow();
                mutex.Unlock();
                Logger::Log(1, "Android Renderer released");
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

                    glClear(GL_COLOR_BUFFER_BIT);

                  /*  vector<int> textureChanges;
                    {
                        PROFILE("Sprites sorting", &rendererProfileManager);

                        if(oldSprites.size() > 0) {
                            std::sort(oldSprites.begin(), oldSprites.end());
                            ITexture* current = oldSprites[0].texture;
                            textureChanges.push_back(0);
                            for(int i=1;i<oldSprites.size();i++) {
                                if(current!=oldSprites[i].texture) {
                                    current = oldSprites[i].texture;
                                    textureChanges.push_back(i);
                                }
                            }
                            textureChanges.push_back(oldSprites.size());
                        }
                    }*/


                    textBox.Draw();


                    {
                       PROFILE("Batch", &rendererProfileManager);



                       batcher->EndBatch();

                       batcher->BeginBatch();
                        /*if(oldSprites.size()>0) {
                            for(int i=1;i<textureChanges.size();i++) {
                                batcher->BeginBatch(oldSprites[textureChanges[i-1]].texture);
                                for(int j=textureChanges[i-1];j<textureChanges[i];j++) {
                                    batcher->DrawSprite(oldSprites[j].x, oldSprites[j].y,
                                                    oldSprites[j].width, oldSprites[j].height,
                                                    oldSprites[j].texRegion, oldSprites[j].angle);
                                }
                                batcher->EndBatch();
                            }
                        }*/
                    }





                    {
                      PROFILE("Swap buffers", &rendererProfileManager);
                    if(contextValid)
                        eglSwapBuffers(display, surface);
                    }



                     frameCounter++;

                     if(frameCounter>60) {
                         frameCounter=0;
                         Logger::Log(0, "FPS: %f", 60.0f/((float)fpsClock.getMSeconds()/1000.0f));
                         fpsClock.reset();
                    }

                    lastTime = currentTime;
                   // }
            //}
            }
            rendererProfileManager.DumpProfileDataToBuffer();
            mutex.Unlock();
            mainLoopCond.Signal();
      //  }
    }
}

void AndroidRenderer::Wait() {
    mainLoopCond.Wait();
    mutex.Lock();
    batcher->SwapSpriteBuffer();
    textBox.SwapTextBuffer();
    mutex.Unlock();

    if(ProfilerManager::profilerEnabled) {
       // if(++profileCounter>400) {
            profileCounter=0;
            //Logger::Log("%s", rendererProfileManager.outputBuffer.Get());
             //DrawString(5, 200, rendererProfileManager.outputBuffer.Get());
            //DrawString(5, 400, "asdas");
            textBox.DrawStr(0, 250, rendererProfileManager.outputBuffer.Get());

        //}
    }

}

void AndroidRenderer::DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height, F32 angle) {
    batcher->DrawSprite(0, x, y, layer, width, height, TextureRegion(), angle);
}

void AndroidRenderer::DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height,
                                 TextureRegion &region, TextureHandle& handle, F32 angle) {
    batcher->DrawSprite(handle.Get(), x, y, layer, width, height, region, angle);
}

void AndroidRenderer::DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height, U8 r, U8 g, U8 b, U8 a, F32 angle) {
    batcher->DrawSprite(r, g, b, a, x, y, layer, width, height, angle);
}

void AndroidRenderer::DrawString(int x, int y, const char * str) {
    textBox.DrawStr(x, y, (char*)str);
}
