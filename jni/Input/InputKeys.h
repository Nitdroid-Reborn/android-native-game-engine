#ifndef INPUTKEYS_H
#define INPUTKEYS_H
#include <Utils/EngineTypes.h>
typedef U8 EngineKeyCode;
typedef U8 EnginePointerId;

#define ENGINE_TOUCH_MAX_POINTERS 10

/*static EngineKeyCode ENGINE_KEYCODE_UNKNOWN         = 0x0000000000000001LL;
static EngineKeyCode ENGINE_KEYCODE_UP              = 0x0000000000000002LL;
static EngineKeyCode ENGINE_KEYCODE_DOWN            = 0x0000000000000004LL;
static EngineKeyCode ENGINE_KEYCODE_LEFT            = 0x0000000000000008LL;
static EngineKeyCode ENGINE_KEYCODE_RIGHT           = 0x0000000000000010LL;
static EngineKeyCode ENGINE_KEYCODE_CENTER          = 0x0000000000000020LL;
static EngineKeyCode ENGINE_KEYCODE_0               = 0x0000000000000040LL;
static EngineKeyCode ENGINE_KEYCODE_1               = 0x0000000000000080LL;
static EngineKeyCode ENGINE_KEYCODE_2               = 0x0000000000000100LL;
static EngineKeyCode ENGINE_KEYCODE_3               = 0x0000000000000200LL;
static EngineKeyCode ENGINE_KEYCODE_4               = 0x0000000000000400LL;
static EngineKeyCode ENGINE_KEYCODE_5               = 0x0000000000000800LL;
static EngineKeyCode ENGINE_KEYCODE_6               = 0x0000000000001000LL;
static EngineKeyCode ENGINE_KEYCODE_7               = 0x0000000000002000LL;
static EngineKeyCode ENGINE_KEYCODE_8               = 0x0000000000004000LL;
static EngineKeyCode ENGINE_KEYCODE_9               = 0x0000000000008000LL;
static EngineKeyCode ENGINE_KEYCODE_A               = 0x0000000000010000LL;
static EngineKeyCode ENGINE_KEYCODE_B               = 0x0000000000020000LL;
static EngineKeyCode ENGINE_KEYCODE_C               = 0x0000000000040000LL;
static EngineKeyCode ENGINE_KEYCODE_D               = 0x0000000000080000LL;
static EngineKeyCode ENGINE_KEYCODE_E               = 0x0000000000100000LL;
static EngineKeyCode ENGINE_KEYCODE_F               = 0x0000000000200000LL;
static EngineKeyCode ENGINE_KEYCODE_G               = 0x0000000000400000LL;
static EngineKeyCode ENGINE_KEYCODE_H               = 0x0000000000800000LL;
static EngineKeyCode ENGINE_KEYCODE_I               = 0x0000000001000000LL;
static EngineKeyCode ENGINE_KEYCODE_J               = 0x0000000002000000LL;
static EngineKeyCode ENGINE_KEYCODE_K               = 0x0000000004000000LL;
static EngineKeyCode ENGINE_KEYCODE_L               = 0x0000000008000000LL;
static EngineKeyCode ENGINE_KEYCODE_M               = 0x0000000010000000LL;
static EngineKeyCode ENGINE_KEYCODE_N               = 0x0000000020000000LL;
static EngineKeyCode ENGINE_KEYCODE_O               = 0x0000000040000000LL;
static EngineKeyCode ENGINE_KEYCODE_P               = 0x0000000080000000LL;
static EngineKeyCode ENGINE_KEYCODE_Q               = 0x0000000100000000LL;
static EngineKeyCode ENGINE_KEYCODE_R               = 0x0000000200000000LL;
static EngineKeyCode ENGINE_KEYCODE_S               = 0x0000000400000000LL;
static EngineKeyCode ENGINE_KEYCODE_T               = 0x0000000800000000LL;
static EngineKeyCode ENGINE_KEYCODE_U               = 0x0000001000000000LL;
static EngineKeyCode ENGINE_KEYCODE_V               = 0x0000002000000000LL;
static EngineKeyCode ENGINE_KEYCODE_W               = 0x0000004000000000LL;
static EngineKeyCode ENGINE_KEYCODE_X               = 0x0000008000000000LL;
static EngineKeyCode ENGINE_KEYCODE_Y               = 0x0000010000000000LL;
static EngineKeyCode ENGINE_KEYCODE_Z               = 0x0000020000000000LL;
static EngineKeyCode ENGINE_KEYCODE_COMMA           = 0x0000040000000000LL;
static EngineKeyCode ENGINE_KEYCODE_PERIOD          = 0x0000080000000000LL;
static EngineKeyCode ENGINE_KEYCODE_TAB             = 0x0000100000000000LL;
static EngineKeyCode ENGINE_KEYCODE_SPACE           = 0x0000200000000000LL;
static EngineKeyCode ENGINE_KEYCODE_ENTER           = 0x0000400000000000LL;
static EngineKeyCode ENGINE_KEYCODE_DEL             = 0x0000800000000000LL;
static EngineKeyCode ENGINE_KEYCODE_MINUS           = 0x0001000000000000LL;
static EngineKeyCode ENGINE_KEYCODE_EQUALS          = 0x0002000000000000LL;
static EngineKeyCode ENGINE_KEYCODE_LEFT_BRACKET    = 0x0004000000000000LL;
static EngineKeyCode ENGINE_KEYCODE_RIGHT_BRACKET   = 0x0008000000000000LL;
static EngineKeyCode ENGINE_KEYCODE_BACKSLASH       = 0x0010000000000000LL;
static EngineKeyCode ENGINE_KEYCODE_SEMICOLON       = 0x0020000000000000LL;
static EngineKeyCode ENGINE_KEYCODE_APOSTROPHE      = 0x0040000000000000LL;
static EngineKeyCode ENGINE_KEYCODE_SLASH           = 0x0080000000000000LL;
static EngineKeyCode ENGINE_KEYCODE_AT              = 0x0100000000000000LL;
static EngineKeyCode ENGINE_KEYCODE_PLUS            = 0x0200000000000000LL;
static EngineKeyCode ENGINE_KEYCODE_MENU            = 0x0400000000000000LL;
static EngineKeyCode ENGINE_KEYCODE_ESCAPE          = 0x0800000000000000LL;
*/

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


struct KeyEvent {
    EngineKeyCode keyCode;
    EngineKeyActions action;
};

struct TouchEvent {
    EngineTouchActions action;
    EnginePointerId pointerId;
    F32 posX;
    F32 posY;
};
#endif // INPUTKEYS_H
