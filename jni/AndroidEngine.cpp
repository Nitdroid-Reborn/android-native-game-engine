#include "AndroidEngine.h"
#include "FileIO/AndroidFileIO.h"
#include <android/keycodes.h>
#include "Graphics/Texture.h"
#include "Graphics/SpriteBatcher.h"
#include "Utils.h"
#include <unistd.h>

#define PI 3.1415926535897932f

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
    LOGI("Engine created");
    frameCounter = 0;

    this->app = app;

    isQuitting = false;
    isRunning = false;

    if (app->savedState != NULL) {
        state = *(struct saved_state*)app->savedState;
    }

    closeEngine = false;
    initDisp=false;
    termDisp=false;


    //centerKey = new VirtualSingleKey(ENGINE_KEYCODE_CENTER, 500, 200, 50);
    //dpad = new VirtualDPad(125, 355, 100, 25);
}

AndroidEngine::~AndroidEngine() {
    LOGI("Engine destroyed");
}


void AndroidEngine::Initialize() {
//    audioSystem.CreateEngine();
    fileIOSystem = new AndroidFileIO(app->activity->assetManager);
    fileIOSystem->Initialize();

    inputSystem = new Input();
    inputSystem->Initialize();

    renderer = new AndroidRenderer(app);
    renderer->Initialize();


    LOGI("FileIO %p", IFileIO::get());

    texture = new Texture("logo.png");
    spriteBatcher = new SpriteBatcher(100);


/*    virtualInputSystem = new VirtualInput();
    virtualInputSystem->AddKey(centerKey);
    virtualInputSystem->AddKey(dpad);*/



 //   audioSystem.CreateAssetPlayer(app->activity->assetManager, "sound.mp3");
  //  audioSystem.SetAssetPlayerStatus(true);

}

void AndroidEngine::Release() {
    mutex.Lock();
    closeEngine = true;

   // audioSystem.Shutdown();
    fileIOSystem->Release();
    delete fileIOSystem;
    fileIOSystem = NULL;

    inputSystem->Release();
    delete inputSystem;
    inputSystem = NULL;

   /* delete virtualInputSystem;
    virtualInputSystem = NULL;*/


    renderer->Release();
    renderer->WaitForStop();
    LOGI("Renderer stopped");
    mutex.Unlock();
}


void AndroidEngine::ProcessTouchInput(const TouchEvent& event) {
   // mutex.Lock();
    //if(!virtualInputSystem->NewTouchEvent(event))
    inputSystem->ProcessTouchEvent(event);
   // mutex.Unlock();
}

void AndroidEngine::ProcessKeyInput(const KeyEvent& event) {
    mutex.Lock();
    if(event.keyCode==ENGINE_KEYCODE_ESCAPE && event.action == ENGINE_KEYACTION_DOWN) {
        isQuitting=true;
    }
    else
        inputSystem->ProcessKeyEvent(event);
    mutex.Unlock();
}

void AndroidEngine::OnGainedFocus() {
    mutex.Lock();
    isRunning = 1;
    renderer->OnGainedFocus();
    mutex.Unlock();
}

void AndroidEngine::OnLostFocus() {
    mutex.Lock();
    isRunning = 0;
    renderer->OnLostFocus();
    mutex.Unlock();
}

void AndroidEngine::OnSaveState() {
    mutex.Lock();
    app->savedState = malloc(sizeof(struct saved_state));
    *((struct saved_state*)app->savedState) = state;
    app->savedStateSize = sizeof(struct saved_state);
    mutex.Unlock();
}

void AndroidEngine::OnInitWindow() {
    if (app->window != NULL) {
        renderer->OnInitWindow();
    }
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

    if(inputSystem->GetTouchState()->IsPointerDown(ENGINE_POINTER_0)) {
        renderer->DrawSprite(inputSystem->GetTouchState()->GetPointerX(ENGINE_POINTER_0),
                             inputSystem->GetTouchState()->GetPointerY(ENGINE_POINTER_0),
                             120, 120, 0.0);
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

//    if(inputSystem->GetTouchState()->IsPointerJustUp(ENGINE_POINTER_0))
  //      LOGI("up");
}


bool AndroidEngine::IsQuiting() {
    mutex.Lock();
    bool res = isQuitting;
    mutex.Unlock();
    return res;
}


bool AndroidEngine::IsRunning() {
    mutex.Lock();
    bool res = isRunning;
    mutex.Unlock();
    return res;
}


void AndroidEngine::Run() {
    renderer->Start();
    while(1) {
        mutex.Lock();
        if(closeEngine) {
            mutex.Unlock();
            return;
        }

        currentTime = getCurrentTimeInMsec();
        float dt = (float)(currentTime - lastTime);
        fpsClock.update(dt);


        OnFrameStart();
        Update(dt);
        OnFrameEnd();

        renderer->Wait();


      //  LOGI("Stop time: %f", (float)(t2-t1)/1000.0f);

        frameCounter++;

        if(frameCounter>60) {
            frameCounter=0;
            LOGI("MAIN LOOP FPS: %f", 60.0f/((float)fpsClock.getMSeconds()/1000.0f));
            fpsClock.reset();
        }

       lastTime = currentTime;
     //  usleep(15000);
       mutex.UnlockQuasiFIFO(100);
//       usleep(2000);
    }
}

