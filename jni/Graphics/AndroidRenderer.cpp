#ifdef ANDROID
#include "AndroidRenderer.h"
#include <Utils/Utils.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "SpriteBatcher.h"
#include <unistd.h>
#include <ContentManager/AndroidContentManager.h>
#include "TextBox.h"
#include "Font.h"
#include <Utils/Profiler.h>
#include <algorithm>
#include <Scripts/Script.h>
#include "VBO.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "ModelGeometry.h"


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
            EGL_DEPTH_SIZE, 16,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
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

    int attrib_list[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };


    context = eglCreateContext(display, config, manager->GetEGLContext(), attrib_list);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        Logger::Log("Unable to eglMakeCurrent");
        return;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    glEnable(GL_TEXTURE_2D);

    // Initialize GL state.
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);


    //glMatrixMode(GL_PROJECTION);
   // glLoadIdentity();
    //glOrthof(0.0, w, 0.0, h, -1.0, 1.0);
    //gluPerspective(54.0f, (float)w/(float)h, 0.1f, 100.0f);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    glViewport(0, 0, (int) w, (int) h);


    camera->SetProjection(54, (float)w/(float)h, 0.1, 1000.0f);


    mainThreadCamera->Clone(camera);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glDisable(GL_DITHER);

    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);

    glClearColor(0,0,0,1);
    glClearDepthf(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    angle=0.0f;


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

    batcher = new SpriteBatcher(1500);
    batcher->Init();

    SpriteBatcher::batcherProfileManager = &rendererProfileManager;

    myFont.Load("/sdcard/Verdana.ttf", 15);


    textBox.font = &myFont;


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
        luabind::class_<ShaderProgram>("ShaderProgram")
            .def("Bind", &ShaderProgram::Bind)
            .def("Release", &ShaderProgram::Release)
            .def("Link", &ShaderProgram::Link)
            .def("AddShader", (void (ShaderProgram::*)(ShaderHandle))&ShaderProgram::AddShader)
            .def("SetAttributeValue", (void (ShaderProgram::*)(std::string, F32))&ShaderProgram::SetAttributeValue)
            .def("SetAttributeValue", (void (ShaderProgram::*)(std::string, F32, F32))&ShaderProgram::SetAttributeValue)
            .def("SetAttributeValue", (void (ShaderProgram::*)(std::string, F32, F32, F32))&ShaderProgram::SetAttributeValue)
            .def("SetAttributeValue", (void (ShaderProgram::*)(std::string, F32, F32, F32, F32))&ShaderProgram::SetAttributeValue)
            .def("SetAttributeValue", (void (ShaderProgram::*)(std::string, const Vector2&))&ShaderProgram::SetAttributeValue)
            .def("SetAttributeValue", (void (ShaderProgram::*)(std::string, const Vector3&))&ShaderProgram::SetAttributeValue)
            .def("SetAttributeValue", (void (ShaderProgram::*)(std::string, const Vector4&))&ShaderProgram::SetAttributeValue)
            .def("SetUniformValue", (void (ShaderProgram::*)(std::string, F32))&ShaderProgram::SetUniformValue)
            .def("SetUniformValue", (void (ShaderProgram::*)(std::string, F32, F32))&ShaderProgram::SetUniformValue)
            .def("SetUniformValue", (void (ShaderProgram::*)(std::string, F32, F32, F32))&ShaderProgram::SetUniformValue)
            .def("SetUniformValue", (void (ShaderProgram::*)(std::string, F32, F32, F32, F32))&ShaderProgram::SetUniformValue)
            .def("SetUniformValue", (void (ShaderProgram::*)(std::string, const Vector2&))&ShaderProgram::SetUniformValue)
            .def("SetUniformValue", (void (ShaderProgram::*)(std::string, const Vector3&))&ShaderProgram::SetUniformValue)
            .def("SetUniformValue", (void (ShaderProgram::*)(std::string, const Vector4&))&ShaderProgram::SetUniformValue)
            .def("SetUniformValue", (void (ShaderProgram::*)(std::string, const Matrix4x4&))&ShaderProgram::SetUniformValue)
            .def("SetUniformValue", (void (ShaderProgram::*)(std::string, I32))&ShaderProgram::SetUniformValue)
            .def("SetUniformValue", (void (ShaderProgram::*)(std::string, I32, I32))&ShaderProgram::SetUniformValue)
            .def("SetUniformValue", (void (ShaderProgram::*)(std::string, I32, I32, I32))&ShaderProgram::SetUniformValue)
            .def("SetUniformValue", (void (ShaderProgram::*)(std::string, I32, I32, I32, I32))&ShaderProgram::SetUniformValue)
    ];

    luabind::module(L)
    [
        luabind::class_<IRenderer>("Renderer")
            .def("DrawSprite", (void (IRenderer::*)(F32, F32, F32, F32, F32, F32))&IRenderer::DrawSprite)
            .def("DrawSprite", (void (IRenderer::*)(F32, F32, F32, F32, F32, TextureRegion&, TextureHandle&, F32))&IRenderer::DrawSprite)
            .def("DrawString", &IRenderer::DrawString)
            .def("DrawGeometry", (void (IRenderer::*)(ModelGeometryHandle, const Matrix4x4 &, ShaderProgramHandle, const ShaderParametersList*, bool))&IRenderer::DrawGeometry)
            .def("DrawGeometry", (void (IRenderer::*)(ModelGeometryHandle, const Matrix4x4 &, ShaderProgramHandle, bool))&IRenderer::DrawGeometry)
            .def("GetCamera", &IRenderer::GetCamera)
            .def("Flush", &IRenderer::Wait)
            .scope
            [
                luabind::def("Get", IRenderer::get)
            ]
            .enum_("SpriteLayers")
            [
                luabind::value("NORMAL_LAYER", NORMAL_LAYER)
            ]
    ];

    luabind::module(L)
    [
        luabind::class_<Camera>("Camera")
            .def("GetPosition", &Camera::GetPosition)
            .def("GetDirection", &Camera::GetDirection)
            .def("GetUpVector", &Camera::GetUpVector)
            .def("GetLeftVector", &Camera::GetLeftVector)
            .def("GetVievMatrix", &Camera::GetViewMatrix)
            .def("GetProjectionMatrix", &Camera::GetProjectionMatrix)
            .def("SetPosition", &Camera::SetPosition)
            .def("SetDirection", &Camera::SetDirection)
            .def("SetUpVector", &Camera::SetUpVector)
            .def("SetHorizontalAngle", &Camera::SetHorizontalAngle)
            .def("SetVerticalAngle", &Camera::SetVerticalAngle)
            .def("GetHorizontalAngle", &Camera::GetHorizontalAngle)
            .def("GetVerticalAngle", &Camera::GetVerticalAngle)
            .def("MoveForward", &Camera::MoveFoward)
            .def("MoveLeft", &Camera::MoveLeft)
            .def("MoveUp", &Camera::MoveUp)
            .def("RotateLeft", &Camera::RotateLeft)
            .def("RotateUp", &Camera::RotateUp)
            .def("SetProjection", &Camera::SetProjection)
    ];

    luabind::module(L)
    [
        luabind::class_<ShaderParametersList>("ShaderParametersList")
            .def(luabind::constructor<>())
            .def("Add", (void (ShaderParametersList::*)(std::string, const Matrix4x4&))&ShaderParametersList::Add)
            .def("Add", (void (ShaderParametersList::*)(std::string, const Vector2&))&ShaderParametersList::Add)
            .def("Add", (void (ShaderParametersList::*)(std::string, const Vector3&))&ShaderParametersList::Add)
            .def("Add", (void (ShaderParametersList::*)(std::string, const Vector4&))&ShaderParametersList::Add)
            .def("Add", (void (ShaderParametersList::*)(std::string, F32))&ShaderParametersList::Add)
            .def("Add", (void (ShaderParametersList::*)(std::string, I32))&ShaderParametersList::Add)
            .def("Clear", &ShaderParametersList::Clear)
    ];

    ScriptSourceHandle initGraphicsScriptSrc = IContentManager::get()->GetScriptSourceManager()->GetScriptSource(":initGraphics.lua");
    Script initGraphicsScript;
    initGraphicsScript.Run(initGraphicsScriptSrc.Get());

    camera = new Camera();
    mainThreadCamera = new Camera();

    camera->SetPosition(Vector3(0,0,0));
    camera->SetDirection(Vector3(0,0,-1));
    camera->SetUpVector(Vector3(0,1,0));


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
    singleton = NULL;

    delete camera;
    delete mainThreadCamera;

    camera = mainThreadCamera = NULL;
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

            if(contextValid) {
           // if(active) {
               // mutex.Unlock();
               // pauseConditionalVariable.Wait();

            {
                    PROFILE("Render", &rendererProfileManager);
                    currentTime = GetCurrentTimeInMsec();
                    float dt = (float)(currentTime - lastTime);
                    fpsClock.update(dt);

                    //if(contextValid) {

                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


                    camera->Update();

                    {
                        PROFILE("Model rendering", &rendererProfileManager);

                        for(int i=0;i<oldGeometry.size();i++) {
                            oldGeometry[i].geometry->Draw(camera, oldGeometry[i].worldMatrix, oldGeometry[i].shaderProgram, oldGeometry[i].shaderParameters);
                        }

                        for(int i=0;i<oldAlphaGeometry.size();i++) {
                            oldAlphaGeometry[i].geometry->Draw(camera, oldAlphaGeometry[i].worldMatrix,
                                                          oldAlphaGeometry[i].shaderProgram, oldAlphaGeometry[i].shaderParameters);
                        }
                    }


                    {
                       PROFILE("Batch", &rendererProfileManager);


                       glDisable(GL_DEPTH_TEST);
                       batcher->EndBatch();

                       batcher->BeginBatch();

                       glEnable(GL_DEPTH_TEST);

                    }

                    angle+=1.1;

                    {
                      PROFILE("Swap buffers", &rendererProfileManager);
                    if(contextValid)
                        eglSwapBuffers(display, surface);
                    }
                }



                     frameCounter++;

                     if(frameCounter>60) {
                         frameCounter=0;
                         lastFps = 60.0f/((float)fpsClock.getMSeconds()/1000.0f);
                         Logger::Log(0, "FPS: %f", 60.0f/((float)fpsClock.getMSeconds()/1000.0f));
                         fpsClock.reset();
                     }
                     char temp[255];
                     sprintf(temp, "FPS: %f", lastFps);
                     DrawString(650, 400, temp);

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
    oldGeometry = geometry;
    oldAlphaGeometry = alphaGeometry;

    alphaGeometry.clear();
    geometry.clear();
    camera->Clone(mainThreadCamera);
    mutex.Unlock();


    if(ProfilerManager::profilerEnabled) {
       // if(++profileCounter>400) {
            profileCounter=0;
            Logger::Log("%s", rendererProfileManager.outputBuffer.Get());
             //DrawString(5, 200, rendererProfileManager.outputBuffer.Get());
            //DrawString(5, 400, "asdas");
            //textBox.DrawStr(0, 250, rendererProfileManager.outputBuffer.Get());

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


void AndroidRenderer::DrawGeometry(ModelGeometryHandle geometry, const Matrix4x4 &worldMatrix,
                              ShaderProgramHandle shaderProgram, const ShaderParametersList* shaderParameters,
                              bool transparent) {
    GeometryInstance gi;
    gi.geometry = geometry.Get();
    gi.worldMatrix = worldMatrix;
    gi.shaderProgram = shaderProgram.Get();
    gi.shaderParameters = ShaderParametersList(*shaderParameters);

    if(!transparent)
        this->geometry.push_back(gi);
    else
        this->alphaGeometry.push_back(gi);
}

void AndroidRenderer::DrawGeometry(ModelGeometryHandle geometry, const Matrix4x4 &worldMatrix,
                              ShaderProgramHandle shaderProgram,
                              bool transparent) {
    GeometryInstance gi;
    gi.geometry = geometry.Get();
    gi.worldMatrix = worldMatrix;
    gi.shaderProgram = shaderProgram.Get();

    if(!transparent)
        this->geometry.push_back(gi);
    else
        this->alphaGeometry.push_back(gi);
}
#endif
