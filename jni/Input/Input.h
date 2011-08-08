#ifndef INPUT_H
#define INPUT_H

#include "EngineTypes.h"

enum EngineKeyCodes {
    ENGINE_KEYCODE_UNKNOWN         = 0,
    ENGINE_KEYCODE_UP              = 1,
    ENGINE_KEYCODE_DOWN            = 2,
    ENGINE_KEYCODE_LEFT            = 3,
    ENGINE_KEYCODE_RIGHT           = 4,
    ENGINE_KEYCODE_CENTER          = 5,
    ENGINE_KEYCODE_0               = 6,
    ENGINE_KEYCODE_1               = 7,
    ENGINE_KEYCODE_2               = 8,
    ENGINE_KEYCODE_3               = 9,
    ENGINE_KEYCODE_4               = 10,
    ENGINE_KEYCODE_5               = 11,
    ENGINE_KEYCODE_6               = 12,
    ENGINE_KEYCODE_7               = 13,
    ENGINE_KEYCODE_8               = 14,
    ENGINE_KEYCODE_9               = 15,
    ENGINE_KEYCODE_A               = 16,
    ENGINE_KEYCODE_B               = 17,
    ENGINE_KEYCODE_C               = 18,
    ENGINE_KEYCODE_D               = 19,
    ENGINE_KEYCODE_E               = 20,
    ENGINE_KEYCODE_F               = 21,
    ENGINE_KEYCODE_G               = 22,
    ENGINE_KEYCODE_H               = 23,
    ENGINE_KEYCODE_I               = 24,
    ENGINE_KEYCODE_J               = 25,
    ENGINE_KEYCODE_K               = 26,
    ENGINE_KEYCODE_L               = 27,
    ENGINE_KEYCODE_M               = 28,
    ENGINE_KEYCODE_N               = 29,
    ENGINE_KEYCODE_O               = 30,
    ENGINE_KEYCODE_P               = 31,
    ENGINE_KEYCODE_Q               = 32,
    ENGINE_KEYCODE_R               = 33,
    ENGINE_KEYCODE_S               = 34,
    ENGINE_KEYCODE_T               = 35,
    ENGINE_KEYCODE_U               = 36,
    ENGINE_KEYCODE_V               = 37,
    ENGINE_KEYCODE_W               = 38,
    ENGINE_KEYCODE_X               = 39,
    ENGINE_KEYCODE_Y               = 40,
    ENGINE_KEYCODE_Z               = 41,
    ENGINE_KEYCODE_COMMA           = 42,
    ENGINE_KEYCODE_PERIOD          = 43,
    ENGINE_KEYCODE_TAB             = 44,
    ENGINE_KEYCODE_SPACE           = 45,
    ENGINE_KEYCODE_ENTER           = 46,
    ENGINE_KEYCODE_DEL             = 47,
    ENGINE_KEYCODE_MINUS           = 48,
    ENGINE_KEYCODE_EQUALS          = 49,
    ENGINE_KEYCODE_LEFT_BRACKET    = 50,
    ENGINE_KEYCODE_RIGHT_BRACKET   = 51,
    ENGINE_KEYCODE_BACKSLASH       = 52,
    ENGINE_KEYCODE_SEMICOLON       = 53,
    ENGINE_KEYCODE_APOSTROPHE      = 54,
    ENGINE_KEYCODE_SLASH           = 55,
    ENGINE_KEYCODE_AT              = 56,
    ENGINE_KEYCODE_PLUS            = 57
};

enum EngineKeyActions {
    ENGINE_KEYACTION_DOWN    = 0,
    ENGINE_KEYACTION_UP      = 1
};

enum EngineTouchActions {
    ENGINE_TOUCHACTION_DOWN    = 0,
    ENGINE_TOUCHACTION_UP      = 1,
    ENGINE_TOUCHACTION_MOVE    = 2
};


struct KeyEvent {
    EngineKeyCodes keyCode;
    EngineKeyActions action;
};

struct TouchEvent {
    EngineTouchActions action;
    U8 pointerId;
    F32 posX;
    F32 posY;
};

class Input
{
public:

    Input();

    void ProcessKeyEvent();
};

#endif // INPUT_H
