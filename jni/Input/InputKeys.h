#ifndef INPUTKEYS_H
#define INPUTKEYS_H
#include <Utils/EngineTypes.h>
typedef U8 EngineKeyCode;
typedef U8 EnginePointerId;

#define ENGINE_TOUCH_MAX_POINTERS 10

static EngineKeyCode ENGINE_KEYCODE_UNKNOWN         = 0;
static EngineKeyCode ENGINE_KEYCODE_UP              = 1;
static EngineKeyCode ENGINE_KEYCODE_DOWN            = 2;
static EngineKeyCode ENGINE_KEYCODE_LEFT            = 3;
static EngineKeyCode ENGINE_KEYCODE_RIGHT           = 4;
static EngineKeyCode ENGINE_KEYCODE_CENTER          = 5;
static EngineKeyCode ENGINE_KEYCODE_0               = 6;
static EngineKeyCode ENGINE_KEYCODE_1               = 7;
static EngineKeyCode ENGINE_KEYCODE_2               = 8;
static EngineKeyCode ENGINE_KEYCODE_3               = 9;
static EngineKeyCode ENGINE_KEYCODE_4               = 10;
static EngineKeyCode ENGINE_KEYCODE_5               = 11;
static EngineKeyCode ENGINE_KEYCODE_6               = 12;
static EngineKeyCode ENGINE_KEYCODE_7               = 13;
static EngineKeyCode ENGINE_KEYCODE_8               = 14;
static EngineKeyCode ENGINE_KEYCODE_9               = 15;
static EngineKeyCode ENGINE_KEYCODE_A               = 16;
static EngineKeyCode ENGINE_KEYCODE_B               = 17;
static EngineKeyCode ENGINE_KEYCODE_C               = 18;
static EngineKeyCode ENGINE_KEYCODE_D               = 19;
static EngineKeyCode ENGINE_KEYCODE_E               = 20;
static EngineKeyCode ENGINE_KEYCODE_F               = 21;
static EngineKeyCode ENGINE_KEYCODE_G               = 22;
static EngineKeyCode ENGINE_KEYCODE_H               = 23;
static EngineKeyCode ENGINE_KEYCODE_I               = 24;
static EngineKeyCode ENGINE_KEYCODE_J               = 25;
static EngineKeyCode ENGINE_KEYCODE_K               = 26;
static EngineKeyCode ENGINE_KEYCODE_L               = 27;
static EngineKeyCode ENGINE_KEYCODE_M               = 28;
static EngineKeyCode ENGINE_KEYCODE_N               = 29;
static EngineKeyCode ENGINE_KEYCODE_O               = 30;
static EngineKeyCode ENGINE_KEYCODE_P               = 31;
static EngineKeyCode ENGINE_KEYCODE_Q               = 32;
static EngineKeyCode ENGINE_KEYCODE_R               = 33;
static EngineKeyCode ENGINE_KEYCODE_S               = 34;
static EngineKeyCode ENGINE_KEYCODE_T               = 35;
static EngineKeyCode ENGINE_KEYCODE_U               = 36;
static EngineKeyCode ENGINE_KEYCODE_V               = 37;
static EngineKeyCode ENGINE_KEYCODE_W               = 38;
static EngineKeyCode ENGINE_KEYCODE_X               = 39;
static EngineKeyCode ENGINE_KEYCODE_Y               = 40;
static EngineKeyCode ENGINE_KEYCODE_Z               = 41;
static EngineKeyCode ENGINE_KEYCODE_COMMA           = 42;
static EngineKeyCode ENGINE_KEYCODE_PERIOD          = 43;
static EngineKeyCode ENGINE_KEYCODE_TAB             = 44;
static EngineKeyCode ENGINE_KEYCODE_SPACE           = 45;
static EngineKeyCode ENGINE_KEYCODE_ENTER           = 46;
static EngineKeyCode ENGINE_KEYCODE_DEL             = 47;
static EngineKeyCode ENGINE_KEYCODE_MINUS           = 48;
static EngineKeyCode ENGINE_KEYCODE_EQUALS          = 49;
static EngineKeyCode ENGINE_KEYCODE_LEFT_BRACKET    = 50;
static EngineKeyCode ENGINE_KEYCODE_RIGHT_BRACKET   = 51;
static EngineKeyCode ENGINE_KEYCODE_BACKSLASH       = 52;
static EngineKeyCode ENGINE_KEYCODE_SEMICOLON       = 53;
static EngineKeyCode ENGINE_KEYCODE_APOSTROPHE      = 54;
static EngineKeyCode ENGINE_KEYCODE_SLASH           = 55;
static EngineKeyCode ENGINE_KEYCODE_AT              = 56;
static EngineKeyCode ENGINE_KEYCODE_PLUS            = 57;
static EngineKeyCode ENGINE_KEYCODE_MENU            = 58;
static EngineKeyCode ENGINE_KEYCODE_ESCAPE          = 59;

static EnginePointerId ENGINE_POINTER_0             = 0;
static EnginePointerId ENGINE_POINTER_1             = 1;
static EnginePointerId ENGINE_POINTER_2             = 2;
static EnginePointerId ENGINE_POINTER_3             = 3;
static EnginePointerId ENGINE_POINTER_4             = 4;
static EnginePointerId ENGINE_POINTER_5             = 5;
static EnginePointerId ENGINE_POINTER_6             = 6;
static EnginePointerId ENGINE_POINTER_7             = 7;
static EnginePointerId ENGINE_POINTER_8             = 8;
static EnginePointerId ENGINE_POINTER_9             = 9;
static EnginePointerId ENGINE_POINTER_10            = 10;

enum EngineKeyActions {
    ENGINE_KEYACTION_DOWN    = 0,
    ENGINE_KEYACTION_UP      = 1
};

enum EngineTouchActions {
    ENGINE_TOUCHACTION_DOWN    = 0,
    ENGINE_TOUCHACTION_UP      = 1,
    ENGINE_TOUCHACTION_MOVE    = 2
};

//! Key event description
struct KeyEvent {
    //! Key code
    EngineKeyCode keyCode;
    //! Key action
    EngineKeyActions action;
};

//! Touch event description
struct TouchEvent {
    //! Type of action
    EngineTouchActions action;
    //! Id of pointer
    EnginePointerId pointerId;
    //! X position of pointer
    F32 posX;
    //! Y position of pointer
    F32 posY;
};

struct AccelEvent {
    F32 x;
    F32 y;
    F32 z;
};

#endif // INPUTKEYS_H
