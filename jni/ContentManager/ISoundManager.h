#ifndef ISOUNDMANAGER_H
#define ISOUNDMANAGER_H

#include "Handle.h"
#include "HandleManager.h"
#include <Audio/ISoundFactory.h>
#include <Scripts/oolua/oolua.h>


struct tagSound {};


typedef Handle<ISound, ISoundFactory, tagSound> SoundHandle;
typedef HandleManager<ISound, ISoundFactory, tagSound> SoundHandleManager;

class ISoundManager {
public:
    ISoundManager(){}
    virtual ~ISoundManager(){}

    virtual SoundHandle GetSound(const char* filename)=0;
    virtual void ReleaseSound(SoundHandle& handle)=0;
};


OOLUA_PROXY_CLASS(SoundHandle)
        OOLUA_TYPEDEFS
                OOLUA::Equal_op,
                OOLUA::Not_equal_op
        OOLUA_END_TYPES
    OOLUA_ONLY_DEFAULT_CONSTRUCTOR
    OOLUA_MEM_FUNC_0(ISound*, Get)
OOLUA_CLASS_END

OOLUA_PROXY_CLASS(ISoundManager)
    OOLUA_MEM_FUNC(SoundHandle, GetSound, const char*)
    OOLUA_MEM_FUNC(void, ReleaseSound, SoundHandle&)
    OOLUA_TYPEDEFS Abstract OOLUA_END_TYPES
OOLUA_CLASS_END
#endif // ISOUNDMANAGER_H
