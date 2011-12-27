#include "Input.h"
#include <Utils/Utils.h>

Input* Input::singleton = NULL;

Input::Input()
{
    pendingKeyEvents.reserve(10);
    pendingTouchEvents.reserve(10);
}


bool Input::Initialize() {
    ASSERT(!singleton, "Input system already initialized");
    singleton = this;

    ScriptManager* manager = ScriptManager::Get();

    lua_State* L = manager->getState();

    luabind::module(L)
    [
        luabind::class_<KeysState>("KeysState")
            .def("IsKeyPressed", &KeysState::IsKeyPressed)
            .def("IsKeyJustPressed", &KeysState::IsKeyJustReleased)
            .def("IsKeyJustReleased", &KeysState::IsKeyJustReleased)
    ];

    luabind::module(L)
    [
        luabind::class_<TouchState>("TouchState")
            .def("IsPointerDown", &TouchState::IsPointerDown)
            .def("IsPointerJustDown", &TouchState::IsPointerJustDown)
            .def("IsPointerJustUp", &TouchState::IsPointerJustUp)
            .def("GetPointerX", &TouchState::GetPointerX)
            .def("GetPointerY", &TouchState::GetPointerY)
    ];

    luabind::module(L)
    [
        luabind::class_<AccelerometerState>("AccelState")
            .def("GetAcceleration", &AccelerometerState::GetAcceleration)
            .def("GetRawAcceleration", &AccelerometerState::GetRawAcceleration)
    ];

    luabind::module(L)
    [
        luabind::class_<Input>("Input")
            .def("GetKeys", &Input::GetKeyState)
            .def("GetTouch", &Input::GetTouchState)
            .def("GetAccel", &Input::GetAccelState)
            .scope
            [
                luabind::def("Get", Input::get)
            ]
            .enum_("KeyKodes")
            [
                luabind::value("POINTER_0", 0),
                luabind::value("POINTER_1", 1),
                luabind::value("POINTER_2", 2),
                luabind::value("POINTER_3", 3),
                luabind::value("POINTER_4", 4),
                luabind::value("POINTER_5", 5),
                luabind::value("POINTER_6", 6),
                luabind::value("POINTER_7", 7),
                luabind::value("POINTER_8", 8),
                luabind::value("POINTER_9", 9),
                luabind::value("POINTER_10", 10),
                luabind::value("KEY_UNKNOW", 0),
                luabind::value("KEY_UP", 1),
                luabind::value("KEY_DOWN", 2),
                luabind::value("KEY_LEFT", 3),
                luabind::value("KEY_RIGHT", 4),
                luabind::value("KEY_CENTER", 5),
                luabind::value("KEY_0", 6),
                luabind::value("KEY_1", 7),
                luabind::value("KEY_2", 8),
                luabind::value("KEY_3", 9),
                luabind::value("KEY_4", 10),
                luabind::value("KEY_5", 11),
                luabind::value("KEY_6", 12),
                luabind::value("KEY_7", 13),
                luabind::value("KEY_8", 14),
                luabind::value("KEY_9", 15),
                luabind::value("KEY_A", 16),
                luabind::value("KEY_B", 17),
                luabind::value("KEY_C", 18),
                luabind::value("KEY_D", 19),
                luabind::value("KEY_E", 20),
                luabind::value("KEY_F", 21),
                luabind::value("KEY_G", 22),
                luabind::value("KEY_H", 23),
                luabind::value("KEY_I", 24),
                luabind::value("KEY_J", 25),
                luabind::value("KEY_K", 26),
                luabind::value("KEY_L", 27),
                luabind::value("KEY_M", 28),
                luabind::value("KEY_N", 29),
                luabind::value("KEY_O", 30),
                luabind::value("KEY_P", 31),
                luabind::value("KEY_Q", 32),
                luabind::value("KEY_R", 33),
                luabind::value("KEY_S", 34),
                luabind::value("KEY_T", 35),
                luabind::value("KEY_U", 36),
                luabind::value("KEY_V", 37),
                luabind::value("KEY_W", 38),
                luabind::value("KEY_X", 39),
                luabind::value("KEY_Y", 40),
                luabind::value("KEY_Z", 41),
                luabind::value("KEY_COMMA", 42),
                luabind::value("KEY_PERIOD", 43),
                luabind::value("KEY_TAB", 44),
                luabind::value("KEY_SPACE", 45),
                luabind::value("KEY_ENTER", 46),
                luabind::value("KEY_DEL", 47),
                luabind::value("KEY_MINUS", 48),
                luabind::value("KEY_EQUALS", 49),
                luabind::value("KEY_LEFT_BRACKET", 50),
                luabind::value("KEY_RIGHT_BRACKET", 51),
                luabind::value("KEY_BACKSLASH", 52),
                luabind::value("KEY_SEMICOLON", 53),
                luabind::value("KEY_APOSTROPHE", 54),
                luabind::value("KEY_SLASH", 55),
                luabind::value("KEY_AT", 56),
                luabind::value("KEY_PLUS", 57),
                luabind::value("KEY_MENU", 58),
                luabind::value("KEY_ESCAPE", 59)
            ]

    ];

    Logger::Log(1, "Input system initialized");
    return true;
}

bool Input::Release() {
    singleton = NULL;
    Logger::Log(1, "Input system released");
    return true;
}

const KeysState* Input::GetKeyState() const {
    return &keyState;
}

const TouchState* Input::GetTouchState() const {
    return &touchState;
}

const AccelerometerState* Input::GetAccelState() const {
    return &accelerometerState;
}

void Input::ProcessKeyEvent(const KeyEvent &event) {
    mutex.Lock();
    pendingKeyEvents.push_back(event);
    mutex.Unlock();
}

void Input::ProcessTouchEvent(const TouchEvent &event) {
    mutex.Lock();
    pendingTouchEvents.push_back(event);
    mutex.Unlock();
}

void Input::ProcessAccelerometerEvent(const AccelEvent &event) {
    mutex.Lock();
    pendingAccelEvents.push_back(event);
    mutex.Unlock();
}


void Input::EndFrame() {
    keyState.EndFrame();
    touchState.EndFrame();
}

void Input::StartFrame() {
    mutex.Lock();
    for(int i=0;i<pendingKeyEvents.size();i++) {
        keyState.NewEvent(pendingKeyEvents[i]);
    }
    pendingKeyEvents.clear();

    for(int i=0;i<pendingTouchEvents.size();i++) {
        touchState.NewEvent(pendingTouchEvents[i]);
    }
    pendingTouchEvents.clear();

    for(int i=0;i<pendingAccelEvents.size();i++) {
        accelerometerState.NewEvent(pendingAccelEvents[i]);
    }

    pendingAccelEvents.clear();

    mutex.Unlock();

    keyState.StartFrame();
    touchState.StartFrame();
}
