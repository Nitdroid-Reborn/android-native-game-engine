#ifndef INPUT_H
#define INPUT_H

#include "InputKeys.h"
#include "KeysState.h"
#include "TouchState.h"
#include <vector>
#include <Core/Mutex.h>
#include <Scripts/ScriptManager.h>

using namespace std;

class Input
{
public:
    Input();
    virtual ~Input() {}

    virtual bool Initialize();
    virtual bool Release();

    virtual void ProcessKeyEvent(const KeyEvent&);
    virtual void ProcessTouchEvent(const TouchEvent&);

    virtual void StartFrame();
    virtual void EndFrame();

    const KeysState* GetKeyState() const;
    const TouchState* GetTouchState() const;

    static Input* get() {return singleton;}

protected:
    static Input* singleton;

private:
    KeysState keyState;
    TouchState touchState;
    vector<KeyEvent> pendingKeyEvents;
    vector<TouchEvent> pendingTouchEvents;
    Mutex mutex;
};


int InputGet(lua_State*l);

/*OOLUA_PROXY_CLASS(Input)
    OOLUA_NO_TYPEDEFS
    OOLUA_ONLY_DEFAULT_CONSTRUCTOR
    OOLUA_MEM_FUNC_0_CONST(const KeysState*, GetKeyState)
    OOLUA_MEM_FUNC_0_CONST(const TouchState*, GetTouchState)
OOLUA_CLASS_END
*/
#endif // INPUT_H
