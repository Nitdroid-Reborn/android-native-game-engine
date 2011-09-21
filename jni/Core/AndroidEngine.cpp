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


    //centerKey = new VirtualSingleKey(ENGINE_KEYCODE_CENTER, 500, 200, 50);
    //dpad = new VirtualDPad(125, 355, 100, 25);
}

AndroidEngine::~AndroidEngine() {
    Logger::Log(1, "Engine destroyed");
}

void func() {
    Logger::Log("boost thread");
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


    volume = 1.0f;
    angle = 0.0f;


    //audioSystem->PlayMusic("/sdcard/music.mp3", 1.0);

 //   sound1 = contentManager->GetSoundManager()->GetSound("/sdcard/violin.wav");


    texture = IContentManager::get()->GetTextureManager()->GetTexture("logo.png");

    lastTime = GetCurrentTimeInMsec();


    ScriptManager* manager = ScriptManager::Get();

    luabind::module(manager->getState()) [

            luabind::def("Log", &Logger::LuaLog)
    ];


   // delete script;

    U32 size = fileIOSystem->GetAssetSize("script.lua");

    char * scriptText = new char[size];
    std::string scriptTextStd;
    fileIOSystem->ReadAsset("script.lua", scriptText, size);

    for(int i=0;i<size;i++) {
        scriptTextStd+=scriptText[i];
    }
    delete [] scriptText;



    script = new Script();
    script->runString(scriptTextStd);

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
/*    virtualInputSystem = new VirtualInput();
    virtualInputSystem->AddKey(centerKey);
    virtualInputSystem->AddKey(dpad);*/
}

void AndroidEngine::Release() {
   // mutex.Lock();

    delete script;

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

    contentManager->Release();
    delete contentManager;
    contentManager = NULL;
    audioSystem = NULL;

    fileIOSystem->Release();
    delete fileIOSystem;
    fileIOSystem = NULL;



   /* delete virtualInputSystem;
    virtualInputSystem = NULL;*/




   // mutex.UnlockQuasiFIFO();
}


void AndroidEngine::ProcessTouchInput(const TouchEvent& event) {
    //if(!virtualInputSystem->NewTouchEvent(event))
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
   /* vector<KeyEvent> events = virtualInputSystem->GetEvents();
    for(int i=0;i<events.size();++i) {
        inputSystem->ProcessKeyEvent(events[i]);
    }*/

    inputSystem->StartFrame();
}

void AndroidEngine::OnFrameEnd() {
    inputSystem->EndFrame();
}

void AndroidEngine::Update(float dt) {
    this->dt = dt;

    if(ProfilerManager::profilerEnabled)
        renderer->DrawString(5, 5, mainLoopProfileManager.outputBuffer.Get());

    if(inputSystem->GetTouchState()->IsPointerDown(ENGINE_POINTER_0)) {


        renderer->DrawSprite(inputSystem->GetTouchState()->GetPointerX(ENGINE_POINTER_0),
                             inputSystem->GetTouchState()->GetPointerY(ENGINE_POINTER_0),
                             180, 180, 0.0);
    }
    if(inputSystem->GetTouchState()->IsPointerDown(ENGINE_POINTER_1)) {
        renderer->DrawSprite(inputSystem->GetTouchState()->GetPointerX(ENGINE_POINTER_1),
                             inputSystem->GetTouchState()->GetPointerY(ENGINE_POINTER_1),
                             120, 120, 0.0);
    }
    if(inputSystem->GetTouchState()->IsPointerDown(ENGINE_POINTER_2)) {
        renderer->DrawSprite(inputSystem->GetTouchState()->GetPointerX(ENGINE_POINTER_2),
                             inputSystem->GetTouchState()->GetPointerY(ENGINE_POINTER_2),
                             120, 120, 0.0);
    }
    if(inputSystem->GetTouchState()->IsPointerDown(ENGINE_POINTER_3)) {
        renderer->DrawSprite(inputSystem->GetTouchState()->GetPointerX(ENGINE_POINTER_3),
                             inputSystem->GetTouchState()->GetPointerY(ENGINE_POINTER_3),
                             120, 120, 0.0);


    }

    script->callFunction("update", dt);

    /*if(inputSystem->GetTouchState()->IsPointerJustDown(ENGINE_POINTER_0))
        sound1.Get()->Play();

    if(inputSystem->GetTouchState()->IsPointerJustDown(ENGINE_POINTER_1))
        sound2.Get()->Play();*/


    TextureRegion tr;
    int counter=0;

   // TextureHandle h2 = IContentManager::get()->GetTextureManager()->GetTexture("logo.png");
  /*  for(int x=20;x<780;x+=40) {
        for( int y= 20; y<460;y+=40) {
            counter++;
            renderer->DrawSprite(x, y, 30, 30, tr, texture, angle);
        }
    }*/

    angle += dt;

    if(inputSystem->GetKeyState()->IsKeyJustPressed(ENGINE_KEYCODE_Z)) {
        volume+=0.1f;
        if(volume>1.0f)volume=1.0f;
        audioSystem->SetMusicVolume(volume);
    }
    else if(inputSystem->GetKeyState()->IsKeyJustPressed(ENGINE_KEYCODE_X)) {
        volume-=0.1f;
        if(volume<0.0f)volume = 0.0f;
        audioSystem->SetMusicVolume(volume);
    }

    if(inputSystem->GetKeyState()->IsKeyJustPressed(ENGINE_KEYCODE_P))
        audioSystem->PlayMusic("/sdcard/music.mp3", 1.0);
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

    frameCounter++;

    if(frameCounter>60) {
        frameCounter=0;
        Logger::Log(0, "MAIN LOOP FPS: %f", 60.0f/((float)fpsClock.getMSeconds()/1000.0f));
        fpsClock.reset();
    }

    lastTime = currentTime;
}
