#ifdef ANDROID
#include <Utils/Utils.h>
#include "AndroidEngine.h"
#include <FileIO/AndroidFileIO.h>
#include <android/keycodes.h>

#include <unistd.h>
#include <Utils/Profiler.h>
#include <Audio/WaveSound.h>
#include <Audio/Sound.h>



extern "C" {
    #include <Scripts/lua/lua.h>
    #include <Scripts/lua/lualib.h>
    #include <Scripts/lua/lauxlib.h>
}
#define PI 3.1415926535897932f

static int profileCounter = 0;
ProfilerManager mainLoopProfileManager;


static void gluPerspective(GLfloat fovy, GLfloat aspect,
                           GLfloat zNear, GLfloat zFar)
{
    GLfloat xmin, xmax, ymin, ymax;

    ymax = zNear * (GLfloat)tan(fovy * PI / 360);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustumx((GLfixed)(xmin * 65536), (GLfixed)(xmax * 65536),
               (GLfixed)(ymin * 65536), (GLfixed)(ymax * 65536),
               (GLfixed)(zNear * 65536), (GLfixed)(zFar * 65536));
}


AndroidEngine::AndroidEngine(android_app* app) : IEngine()
{
    Logger::Log(1, "Engine created");
    frameCounter = 0;

    this->app = app;

    isQuitting = false;
    isRunning = false;

    if (app->savedState != NULL) {
        state = *(struct saved_state*)app->savedState;
    }


    //
}

AndroidEngine::~AndroidEngine() {
    Logger::Log(1, "Engine destroyed");
}


void AndroidEngine::Initialize() {
    scriptManager = new ScriptManager();
    scriptManager->Initialize();

    fileIOSystem = new AndroidFileIO(app->activity->assetManager);
    fileIOSystem->Initialize();

    inputSystem = new Input();
    inputSystem->Initialize();

    contentManager = new AndroidContentManager();
    contentManager->Initialize();

    audioSystem = new AndroidAudioSystem();
    audioSystem->Initialize();

    renderer = new AndroidRenderer(app);
    renderer->Initialize();


    centerKey = new VirtualSingleKey(ENGINE_KEYCODE_CENTER, 700, 80, 50);
    dpad = new VirtualDPad(125, 125, 100, 25);
    virtualInputSystem = new VirtualInput();
    virtualInputSystem->AddKey(centerKey);
    virtualInputSystem->AddKey(dpad);


    volume = 1.0f;
    angle = 0.0f;


    //audioSystem->PlayMusic("/sdcard/music.mp3", 1.0);

 //   sound1 = contentManager->GetSoundManager()->GetSound("/sdcard/violin.wav");


    texture = IContentManager::get()->GetTextureManager()->GetTexture(":logo.png");

    lastTime = GetCurrentTimeInMsec();

    Vector2::RegisterInLua();
    Vector3::RegisterInLua();
    Vector4::RegisterInLua();
    Matrix4x4::RegisterInLua();

    ScriptManager* manager = ScriptManager::Get();

    luabind::module(manager->getState()) [

            luabind::def("Log", &Logger::LuaLog)
    ];


   // delete script;




    ScriptSourceHandle scr = contentManager->GetScriptSourceManager()->GetScriptSource(":script.lua");
    script = new Script();
    script->Run(scr.Get());

    contentManager->GetScriptSourceManager()->ReleaseScriptSource(scr);

    //script2.runString("someSound = IContentManager.Get():GetSoundManager():GetSound('/sdcard/flet.wav'); doSth = function() IContentManager.Get():GetSoundManager():GetSound('/sdcard/flet.wav'):Get():Play(0.5); Logger.Log('dzialam') end;");

    //script2.callFunction("doSth");

    //SoundHandle sound = contentManager->GetSoundManager()->GetSound("/sdcard/flet.wav");


   // Logger::Log("%d",sound.GetReferenceCount());
   // script.callFunction("doSth");*/


    //Logger::Log("Ref count %d", sound2.GetReferenceCount());

    //Run a script
   /*  if(mainState.run_chunk("IContentManager.Get():GetSoundManager():GetSound('/sdcard/violin.wav'):Get():Play(0.5);"))Logger::Log("\n-Script run successfully");///sdcard/test.lua"))
    else Logger::Log("Script error: %s", OOLUA::get_last_error(mainState.get_ptr()).c_str());
*/



  //  acm->texManager.ReleaseTexture(handle);

    //contentManager->LoadTexture("logo.png");

   /* texture = new Texture("logo.png");
    spriteBatcher = new SpriteBatcher(100);

*/

}

void AndroidEngine::Release() {
   // mutex.Lock();

    delete script;
    script = NULL;

    contentManager->GetTextureManager()->ReleaseTexture(texture);

    audioSystem->StopMusic();

    scriptManager->Release();
    delete scriptManager;
    scriptManager = NULL;

    renderer->Release();
    renderer->WaitForStop();
    delete renderer;
    renderer = NULL;    

    audioSystem->Release();
    delete audioSystem;

    inputSystem->Release();
    delete inputSystem;
    inputSystem = NULL;

    delete virtualInputSystem;
    virtualInputSystem = NULL;

    delete dpad;
    delete centerKey;

    contentManager->Release();
    delete contentManager;
    contentManager = NULL;
    audioSystem = NULL;

    fileIOSystem->Release();
    delete fileIOSystem;
    fileIOSystem = NULL;

   // mutex.UnlockQuasiFIFO();
}


void AndroidEngine::ProcessTouchInput(const TouchEvent& event) {
    if(!virtualInputSystem->NewTouchEvent(event))
    inputSystem->ProcessTouchEvent(event);
}

void AndroidEngine::ProcessKeyInput(const KeyEvent& event) {
    if(event.keyCode==ENGINE_KEYCODE_ESCAPE && event.action == ENGINE_KEYACTION_DOWN) {
        isQuitting=true;
    }
    else
        inputSystem->ProcessKeyEvent(event);
}

void AndroidEngine::OnGainedFocus() {
    renderer->OnGainedFocus();
}

void AndroidEngine::OnLostFocus() {
    isRunning = 0;
    renderer->OnLostFocus();
}

void AndroidEngine::OnSaveState() {
    app->savedState = malloc(sizeof(struct saved_state));
    *((struct saved_state*)app->savedState) = state;
    app->savedStateSize = sizeof(struct saved_state);
}

void AndroidEngine::OnInitWindow() {
    if (app->window != NULL) {
        renderer->OnInitWindow();

    }
    isRunning = true;
}

void AndroidEngine::OnTerminateWindow() {
    renderer->OnTerminateWindow();
}

void AndroidEngine::OnPause() {

}

void AndroidEngine::OnResume() {

}


void AndroidEngine::ProcessAccelerometerInput(float x, float y, float z) {
           /* LOGI("accelerometer: x=%f y=%f z=%f",
                    x, y, z);*/
}

void AndroidEngine::OnFrameStart() {
    vector<KeyEvent> events = virtualInputSystem->GetEvents();
    for(int i=0;i<events.size();++i) {
        inputSystem->ProcessKeyEvent(events[i]);
    }

    inputSystem->StartFrame();
}

void AndroidEngine::OnFrameEnd() {
    inputSystem->EndFrame();
}

void AndroidEngine::Update(float dt) {
    this->dt = dt;

    script->callFunction("update", dt);

    virtualInputSystem->Draw();
}


bool AndroidEngine::IsQuiting() {
    bool res = isQuitting;
    return res;
}


bool AndroidEngine::IsRunning() {
    bool res = isRunning;
    return res;
}

void AndroidEngine::SingleFrame() {

    currentTime = GetCurrentTimeInMsec();
    float dt = (float)(currentTime - lastTime);
    fpsClock.update(dt);


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
       // if(++profileCounter>120) {
           // renderer->DrawSprite(400, 240, PROFILER_LAYER_BG, 800, 480, 0, 0, 0, 192);
            //Logger::Log("%s", mainLoopProfileManager.outputBuffer.Get());
            profileCounter = 0;
       // }
        //renderer->DrawString(5, 450, mainLoopProfileManager.outputBuffer.Get());


        //
    }

    frameCounter++;

    if(frameCounter>60) {
        frameCounter=0;
        Logger::Log(0, "MAIN LOOP FPS: %f", 60.0f/((float)fpsClock.getMSeconds()/1000.0f));
        fpsClock.reset();
    }

    lastTime = currentTime;
}

#endif
