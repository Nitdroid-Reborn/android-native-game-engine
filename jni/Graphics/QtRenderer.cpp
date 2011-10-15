#include "qdatastream.h"
#include "qtextstream.h"
#include "qmetatype.h"
#include "qcursor.h"
#include "VBO.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "ModelGeometry.h"

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
    //glEnable(GL_CULL_FACE);
   // glCullFace(GL_BACK);
   // glFrontFace(GL_CCW);

    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DITHER);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

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
        luabind::class_<IRenderer>("Renderer")
            .def("DrawSprite", (void (IRenderer::*)(F32, F32, F32, F32, F32, F32))&IRenderer::DrawSprite)
            .def("DrawSprite", (void (IRenderer::*)(F32, F32, F32, F32, F32, U8, U8, U8, U8, F32))&IRenderer::DrawSprite)
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


    model = new ModelGeometry();
    model->Load("krasnal.ms3d");

    sp = new ShaderProgram();
    vs = new Shader(Shader::VertexShader);
    fs = new Shader(Shader::PixelShader);

    vbo = new VBO(model);

    static const char gVertexShader[] =
        "uniform mat4 mvp;"
        "attribute mediump vec4 vPosition;\n"
        "attribute mediump vec4 vTexCoords;\n"
        "attribute lowp vec4 vColor;\n"
        "varying mediump vec4 texCoords;\n"
        "varying lowp vec4 color;\n"
        "void main() {\n"
        "  gl_Position = mvp * (vPosition);\n"
        "  texCoords = vTexCoords;\n"
        "  color = vColor;\n"
        "}\n";

    static const char gFragmentShader[] =
        "uniform sampler2D textureSampler;\n"
        "varying mediump vec4 texCoords;\n"
        "varying lowp vec4 color;\n"
        "void main() {\n"
        "  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "}\n";

    vs->CompileSource(gVertexShader);
    fs->CompileSource(gFragmentShader);
    sp->AddShader(vs);
    sp->AddShader(fs);
    sp->Link();

    Logger::Log("num v: %d, num i: %d", model->GetVerticesCount(), model->GetIndicesCount());


    Logger::Log(1, "Qt Renderer initialized");
}

void QtRenderer::Release() {
    myFont.Dispose();
    delete batcher;
    batcher = NULL;;
    singleton = NULL;


    delete sp;
    delete vs;
    delete fs;
    delete vbo;
    delete model;

    sp=NULL;
    vs=NULL;
    fs=NULL;
    vbo=NULL;
    model=NULL;
}

void QtRenderer::Run() {
    {
            PROFILE("Render", &rendererProfileManager);
            currentTime = GetCurrentTimeInMsec();
            float dt = (float)(currentTime - lastTime);
            fpsClock.update(dt);

            //if(contextValid) {

            glClear(GL_COLOR_BUFFER_BIT);

            //textBox.Draw();

            {
               PROFILE("Batch", &rendererProfileManager);


               //batcher->EndBatch();

             //  batcher->BeginBatch();
            }


            sp->Bind();
            sp->EnableAttributeArray("vPosition");
            sp->EnableAttributeArray("vTexCoords");
            sp->EnableAttributeArray("vColor");

            Matrix4x4 mat;
            mat.SetPerspective(54.0f, 800.0f/480.0f, 0.1, 1000);


            Matrix4x4 scale;
            //scale.SetScale(Vector3(2.1 ,2.1, 2.1));
            scale.SetTranslation(Vector3(0, 0, -15));

            mat = mat*scale;

            sp->SetUniformValue("mvp", mat);

            vbo->Bind();
            vbo->SetData(model->GetVerticesCount(), model->GetVertices(),
                         model->GetIndicesCount(), model->GetIndices());
            sp->SetAttributeArray(vbo);

            vbo->Draw(0, model->GetIndicesCount()/3);

            vbo->Release();
            sp->Release();



            {
              PROFILE("Swap buffers", &rendererProfileManager);


              app->swapBuffers();

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


    }

    rendererProfileManager.DumpProfileDataToBuffer();
}

void QtRenderer::Wait() {
    batcher->SwapSpriteBuffer();
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
