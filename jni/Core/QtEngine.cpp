#include <QGLWidget>
#include "QtEngine.h"

#include <Utils/Profiler.h>

#include <Graphics/ModelGeometry.h>
#include <GameObject/RenderableGameObject.h>

static int profileCounter = 0;
ProfilerManager mainLoopProfileManager;


QtEngine::QtEngine(QGLWidget* app) : IEngine()
{
    Logger::Log(1, "Engine created");
    frameCounter = 0;

    this->app = app;

    isQuitting = false;
    isRunning = false;

   /* if (app->savedState != NULL) {
        state = *(struct saved_state*)app->savedState;
    }*/
}

QtEngine::~QtEngine() {
    Logger::Log(1, "Engine destroyed");
}

void QtEngine::Initialize() {

    scriptManager = new ScriptManager();
    scriptManager->Initialize();

    fileIOSystem = new QtFileIO();
    fileIOSystem->Initialize();

    inputSystem = new Input();
    inputSystem->Initialize();

    contentManager = new QtContentManager();
    contentManager->Initialize();


    ScriptManager* manager = ScriptManager::Get();

    luabind::module(manager->getState()) [

            luabind::def("Log", &Logger::LuaLog)
    ];


    audioSystem = new QtAudioSystem();
    audioSystem->Initialize();

    renderer = new QtRenderer(app);
    renderer->Initialize();



    centerKey = new VirtualSingleKey(ENGINE_KEYCODE_CENTER, 700, 80, 50);
    dpad = new VirtualDPad(125, 125, 100, 25);
    virtualInputSystem = new VirtualInput();
    virtualInputSystem->AddKey(centerKey);
    virtualInputSystem->AddKey(dpad);

    gameObjectManager = new GameObjectManager();
    gameObjectManager->Initialize();


    //volume = 1.0f;
    //angle = 0.0f;

    lastTime = GetCurrentTimeInMsec();

    Vector2::RegisterInLua();
    Vector3::RegisterInLua();
    Vector4::RegisterInLua();
    Matrix4x4::RegisterInLua();




    loadingState = true;

    ScriptSourceHandle scr = contentManager->GetScriptSourceManager()->GetScriptSource("script.lua");
    script = new Script();

    script->Run(scr.Get());

    contentManager->GetScriptSourceManager()->ReleaseScriptSource(scr);
}

void QtEngine::Release() {
    delete script;
    script = NULL;



    audioSystem->StopMusic();



    renderer->Release();
    //renderer->WaitForStop();
    delete renderer;
    renderer = NULL;

    audioSystem->Release();
    delete audioSystem;
    audioSystem = NULL;

    inputSystem->Release();
    delete inputSystem;
    inputSystem = NULL;

    delete virtualInputSystem;
    virtualInputSystem = NULL;

    delete dpad;
    delete centerKey;

    scriptManager->Release();
    delete scriptManager;
    scriptManager = NULL;

    gameObjectManager->Release();

    contentManager->Release();
    delete contentManager;
    contentManager = NULL;

    fileIOSystem->Release();
    delete fileIOSystem;
    fileIOSystem = NULL;

}


void QtEngine::ProcessTouchInput(const TouchEvent& event) {
    if(!virtualInputSystem->NewTouchEvent(event))
    inputSystem->ProcessTouchEvent(event);
}

void QtEngine::ProcessKeyInput(const KeyEvent& event) {
    if(event.keyCode==ENGINE_KEYCODE_ESCAPE && event.action == ENGINE_KEYACTION_DOWN) {
        isQuitting=true;
    }
    else
        inputSystem->ProcessKeyEvent(event);
}

void QtEngine::OnGainedFocus() {
    renderer->OnGainedFocus();
}

void QtEngine::OnLostFocus() {
    isRunning = 0;
    renderer->OnLostFocus();
}

void QtEngine::OnSaveState() {
    //app->savedState = malloc(sizeof(struct saved_state));
    //*((struct saved_state*)app->savedState) = state;
    //app->savedStateSize = sizeof(struct saved_state);
}

void QtEngine::OnInitWindow() {
   // if (app->window != NULL) {
        renderer->OnInitWindow();

    //}
    isRunning = true;
}

void QtEngine::OnTerminateWindow() {
    renderer->OnTerminateWindow();
}

void QtEngine::OnPause() {

}

void QtEngine::OnResume() {

}


void QtEngine::ProcessAccelerometerInput(float x, float y, float z) {
           /* LOGI("accelerometer: x=%f y=%f z=%f",
                    x, y, z);*/
}

void QtEngine::OnFrameStart() {
    vector<KeyEvent> events = virtualInputSystem->GetEvents();
    for(int i=0;i<events.size();++i) {
        inputSystem->ProcessKeyEvent(events[i]);
    }

    inputSystem->StartFrame();
}

void QtEngine::OnFrameEnd() {
    inputSystem->EndFrame();
}

void QtEngine::Update(float dt) {
    this->dt = dt;

    script->callFunction("update", dt);

    gameObjectManager->Update(dt);

    virtualInputSystem->Draw();
}


bool QtEngine::IsQuiting() {
    bool res = isQuitting;
    return res;
}


bool QtEngine::IsRunning() {
    bool res = isRunning;
    return res;
}

void QtEngine::SingleFrame() {

    currentTime = GetCurrentTimeInMsec();
    float dt = (float)(currentTime - lastTime);
    fpsClock.update(dt);
    dt/=1000.0f;


    if(loadingState) {
        script->callFunction("loadAssets");
        loadingState=false;
    }
    else {

        {
            PROFILE("Start frame", &mainLoopProfileManager);
            OnFrameStart();
        }

        {
            PROFILE("Update", &mainLoopProfileManager);
            Update(dt);
        }

        {
            PROFILE("Render", &mainLoopProfileManager);
            renderer->Wait();
        }

        {
            PROFILE("End frame", &mainLoopProfileManager);
            OnFrameEnd();
        }

        mainLoopProfileManager.DumpProfileDataToBuffer();
        if(inputSystem->GetKeyState()->IsKeyJustPressed(ENGINE_KEYCODE_MENU)) {
            ProfilerManager::profilerEnabled=!ProfilerManager::profilerEnabled;
        }

        if(ProfilerManager::profilerEnabled) {
            renderer->DrawSprite(400, 240, PROFILER_LAYER_BG, 800, 480, 0, 0, 0, 192);
            profileCounter = 0;
            renderer->DrawString(5, 450, mainLoopProfileManager.outputBuffer.Get());
        }
    }

    frameCounter++;

    if(frameCounter>60) {
        frameCounter=0;
        Logger::Log(0, "MAIN LOOP FPS: %f", 60.0f/((float)fpsClock.getMSeconds()/1000.0f));
        fpsClock.reset();
    }

    lastTime = currentTime;
}
