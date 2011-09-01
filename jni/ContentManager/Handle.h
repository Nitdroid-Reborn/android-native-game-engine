#ifndef HANDLE_H
#define HANDLE_H
#include <Utils/Utils.h>
#include <Utils/Assert.h>

#include <Core/Object.h>

template <typename BASEDATA, typename FACTORY, typename TAG>
class HandleManager;


#define MAX_BITS_INDEX 16
#define MAX_BITS_MAGIC 16
#define MAX_INDEX ( 1<<MAX_BITS_INDEX ) - 1
#define MAX_MAGIC ( 1<<MAX_BITS_MAGIC ) - 1

template <typename BASEDATA, typename FACTORY, typename TAG>
class Handle
{
    union {
        struct {
            U16 index : MAX_BITS_INDEX;
            U16 magic : MAX_BITS_MAGIC;
        };
        U32 handle;
    };

    void Invalidate() {magic=0;}

public:
    Handle() : handle(0) {}
    Handle(const Handle& otherHandle) : handle(otherHandle.handle){
        BASEDATA* o = this->Get();
        if(o) {
            manager->IncrementHandleRefCount(*this);
        }
    }
    ~Handle();

    void Init(U16 index);

    U16 GetIndex() const {return index;}
    U16 GetMagic() const {return magic;}
    U32 GetHandle() const {return handle;}
    bool IsNull() const {return !handle;}
    U16 GetReferenceCount() const {return manager->GetHandleRefCount(*this);}


    operator U32() const {return handle;}

    Handle<BASEDATA, FACTORY, TAG>& operator = (Handle<BASEDATA, FACTORY, TAG>& pOther);
    bool operator == (const Handle<BASEDATA, FACTORY, TAG>& pOther) const;
    bool operator != (const Handle<BASEDATA, FACTORY, TAG>& pOther) const;

    BASEDATA* Get();
    const BASEDATA* Get() const;

    static HandleManager<BASEDATA, FACTORY, TAG>* manager;

    friend class HandleManager<BASEDATA, FACTORY, TAG>;
};

template <typename BASEDATA, typename FACTORY, typename TAG>
void Handle<BASEDATA, FACTORY, TAG>::Init(U16 index) {
    ASSERT(IsNull(), "Handle already initialized");
    ASSERT(index <= MAX_INDEX, "Invalid handle index");

    static U16 autoMagic = 0;
    if(++autoMagic>MAX_MAGIC) {
        autoMagic=1;
    }

    this->index = index;
    this->magic = autoMagic;
}

template <typename BASEDATA, typename FACTORY, typename TAG>
inline Handle<BASEDATA, FACTORY, TAG>& Handle<BASEDATA, FACTORY, TAG>::operator = (Handle<BASEDATA, FACTORY, TAG>& pOther) {
    Log("assigning handle");

    manager->DecrementReferenceCount(*this);


    handle = pOther.handle;

    manager->IncrementHandleRefCount(*this);

    return *this;
}


template <typename BASEDATA, typename FACTORY, typename TAG>
inline bool Handle<BASEDATA, FACTORY, TAG>::operator==(const Handle<BASEDATA, FACTORY, TAG>& pOther) const {
    return (this->index == pOther.index);
}

template <typename BASEDATA, typename FACTORY, typename TAG>
inline bool Handle<BASEDATA, FACTORY, TAG>::operator!=(const Handle<BASEDATA, FACTORY, TAG>& pOther) const {
    return (this->index != pOther.index);
}

template <typename BASEDATA, typename FACTORY, typename TAG>
HandleManager<BASEDATA, FACTORY, TAG>* Handle<BASEDATA, FACTORY, TAG>::manager;

template <typename BASEDATA, typename FACTORY, typename TAG>
Handle<BASEDATA, FACTORY, TAG>::~Handle() {
    if(!IsNull()) {
        manager->Release(*this);
    }
    Log("Handle destructed, left references: %d", GetReferenceCount());
}

template <typename BASEDATA, typename FACTORY, typename TAG>
BASEDATA* Handle<BASEDATA, FACTORY, TAG>::Get() {
    return (BASEDATA*)manager->Dereference(*this);
}

template <typename BASEDATA, typename FACTORY, typename TAG>
const BASEDATA* Handle<BASEDATA, FACTORY, TAG>::Get() const{
    return (const BASEDATA*)manager->Dereference(*this);
}




#endif // HANDLE_H
