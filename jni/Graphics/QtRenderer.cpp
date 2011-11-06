#include <QGLWidget>
#include "VBO.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "ModelGeometry.h"
#include <Scripts/Script.h>
#include "QtRenderer.h"


IRenderer* IRenderer::singleton = NULL;
ProfilerManager rendererProfileManager;
static int profileCounter = 0;

QtRenderer::QtRenderer(QGLWidget* app) : IRenderer()
{
    contextValid = false;
    initWindow = false;
    terminateWindow = false;
    closing = false;
    active = true;
    this->app = app;
}


void QtRenderer::OnInitWindow() {
    Logger::Log(1, "Renderer window initialized");

    glEnable(GL_TEXTURE_2D);

    // Initialize GL state.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    int w,h;
    w = app->width();
    h = app->height();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(0.0, w, 0.0, h, -1.0, 1.0);
    //gluPerspective(54.0f, (float)w/(float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, (int) w, (int) h);

    camera->SetProjection(54, (float)w/(float)h, 0.1, 100.0f);
    camera->SetPosition(Vector3(0,0,0));
    camera->SetDirection(Vector3(0,0,-1));
    camera->SetUpVector(Vector3(0,1,0));

    mainThreadCamera->Clone(camera);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DITHER);

    glClearColor(0,0,0,0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    contextValid=true;
}

void QtRenderer::OnTerminateWindow() {

}

void QtRenderer::OnGainedFocus() {

}

void QtRenderer::OnLostFocus() {

}

void QtRenderer::Initialize() {
    ASSERT(!singleton, "Render system already initialized");
    singleton = this;

    batcher = new SpriteBatcher(1000);
    batcher->Init();

    SpriteBatcher::batcherProfileManager = &rendererProfileManager;

    myFont.Load("/usr/share/fonts/truetype/msttcorefonts/Verdana.ttf", 15);


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
            .def("DrawSprite", (void (IRenderer::*)(F32, F32, F32, F32, F32, U8, U8, U8, U8, F32))&IRenderer::DrawSprite)
            .def("DrawSprite", (void (IRenderer::*)(F32, F32, F32, F32, F32, TextureRegion&, TextureHandle&, F32))&IRenderer::DrawSprite)
            .def("DrawString", &IRenderer::DrawString)
            .def("DrawGeometry", (void (IRenderer::*)(ModelGeometryHandle, const Matrix4x4 &, ShaderProgramHandle, const ShaderParametersList*))&IRenderer::DrawGeometry)
            .def("DrawGeometry", (void (IRenderer::*)(ModelGeometryHandle, const Matrix4x4 &, ShaderProgramHandle))&IRenderer::DrawGeometry)
            .def("GetCamera", &IRenderer::GetCamera)
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


    ScriptSourceHandle initGraphicsScriptSrc = IContentManager::get()->GetScriptSourceManager()->GetScriptSource("initGraphics.lua");
    Script initGraphicsScript;
    initGraphicsScript.Run(initGraphicsScriptSrc.Get());


    /*ShaderProgramHandle shaderProgram;
    ShaderHandle vertexShader;
    ShaderHandle fragmentShader;

    shaderProgram = IContentManager::get()->GetShaderProgramManager()->GetShaderProgram("textured_3d");
    vertexShader = IContentManager::get()->GetShaderManager()->GetShader("shaders/3d.vert");
    fragmentShader = IContentManager::get()->GetShaderManager()->GetShader("shaders/3d.frag");

    shaderProgram.Get()->AddShader(vertexShader.Get());
    shaderProgram.Get()->AddShader(fragmentShader.Get());
    shaderProgram.Get()->Link();


    shaderProgram = IContentManager::get()->GetShaderProgramManager()->GetShaderProgram("spriteBatcher");
    vertexShader = IContentManager::get()->GetShaderManager()->GetShader("shaders/batcher.vert");
    fragmentShader = IContentManager::get()->GetShaderManager()->GetShader("shaders/batcher.frag");

    shaderProgram.Get()->AddShader(vertexShader.Get());
    shaderProgram.Get()->AddShader(fragmentShader.Get());
    shaderProgram.Get()->Link();*/



    camera = new Camera();
    mainThreadCamera = new Camera();

    angle=150.0f;

    Logger::Log(1, "Qt Renderer initialized");
}

void QtRenderer::Release() {
    myFont.Dispose();
    delete batcher;

    batcher = NULL;;
    singleton = NULL;

    delete camera;
    delete mainThreadCamera;

}

void QtRenderer::Run() {
    {
            PROFILE("Render", &rendererProfileManager);
            currentTime = GetCurrentTimeInMsec();
            float dt = (float)(currentTime - lastTime);
            fpsClock.update(dt);

            //if(contextValid) {

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            {
                PROFILE("Camera update", &rendererProfileManager);
                camera->Update();
            }
            //textBox.Draw();

            {
                PROFILE("Model rendering", &rendererProfileManager);


                for(int i=0;i<oldGeometry.size();i++) {
                    oldGeometry[i].geometry->Draw(camera, oldGeometry[i].worldMatrix,
                                                  oldGeometry[i].shaderProgram, oldGeometry[i].shaderParameters);
                }

               /* for(int i=-0;i<=0;i++) {
                    Matrix4x4 translation;
                    translation.SetTranslation(Vector3(i,-1,-5));

                    Matrix4x4 rotation;
                    rotation.SetRotationY(angle);

                    Matrix4x4 world = translation*rotation;

                   // sp = IContentManager::get()->GetShaderProgramManager()->GetShaderProgram("default").Get();
                    mg->Draw(camera, world, sp);
                }*/
            }

            {
               PROFILE("Batch", &rendererProfileManager);


               glDisable(GL_DEPTH_TEST);

               batcher->EndBatch();

               batcher->BeginBatch();

               glEnable(GL_DEPTH_TEST);
            }


            {
              PROFILE("Swap buffers", &rendererProfileManager);


             app->swapBuffers();

             frameCounter++;

             angle+=1.1;

             if(frameCounter>60) {
                 frameCounter=0;
                 Logger::Log(0, "FPS: %f", 60.0f/((float)fpsClock.getMSeconds()/1000.0f));
                 fpsClock.reset();
            }

            lastTime = currentTime;
           // }
    //}
            }


    }

    rendererProfileManager.DumpProfileDataToBuffer();
}

void QtRenderer::Wait() {
    batcher->SwapSpriteBuffer();
    camera->Clone(mainThreadCamera);

    oldGeometry = geometry;
    geometry.clear();
    //textBox.SwapTextBuffer();

    Run();

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

void QtRenderer::DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height, F32 angle) {
   batcher->DrawSprite(0, x, y, layer, width, height, TextureRegion(), angle);
}

void QtRenderer::DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height,
                                 TextureRegion &region, TextureHandle& handle, F32 angle) {
    batcher->DrawSprite(handle.Get(), x, y, layer, width, height, region, angle);
}

void QtRenderer::DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height, U8 r, U8 g, U8 b, U8 a, F32 angle) {
    batcher->DrawSprite(r, g, b, a, x, y, layer, width, height, angle);
}

void QtRenderer::DrawString(int x, int y, const char * str) {
    textBox.DrawStr(x, y, (char*)str);
}


void QtRenderer::DrawGeometry(ModelGeometryHandle geometry, const Matrix4x4 &worldMatrix,
                              ShaderProgramHandle shaderProgram, const ShaderParametersList* shaderParameters) {
    GeometryInstance gi;
    gi.geometry = geometry.Get();
    gi.worldMatrix = worldMatrix;
    gi.shaderProgram = shaderProgram.Get();
    gi.shaderParameters = ShaderParametersList(*shaderParameters);

    this->geometry.push_back(gi);
}

void QtRenderer::DrawGeometry(ModelGeometryHandle geometry, const Matrix4x4 &worldMatrix,
                              ShaderProgramHandle shaderProgram) {
    GeometryInstance gi;
    gi.geometry = geometry.Get();
    gi.worldMatrix = worldMatrix;
    gi.shaderProgram = shaderProgram.Get();
    this->geometry.push_back(gi);
}

