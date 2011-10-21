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

//! Generic content handle
/*!
  Used to reference content resources from content manager safetly, with no direct use of pointers.
  Each handle consist of two 16-bits integers - unique magic number and index pointing to resource.
  Handles are managed by handle manager which internally saves also magic numbers of each handle and used it to check corectness of handle.
  Each assign or copy of handle increases reference counter of resource to allow safetly
  remove of resource when no longer needed.
  Each handle is template wich needs 3 types
  BASEDATA - is base class of resource handled by handle
  FACTORY - is abstract factory that creates BASEDATA objects
  TAG - empty structure that assure type checking
*/
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
    //! Default constructor
    /*!
      Creates invalid handle.
      */
    Handle() : handle(0) {}
    //! Copy constructor
    /*!
      Creates handle which refers to the same resource as otherHandle and increases
      number of references to resource
      */
    Handle(const Handle& otherHandle) : handle(otherHandle.handle){
        BASEDATA* o = this->Get();
        if(o) {
            manager->IncrementHandleRefCount(*this);
        }
    }

    //! Destructor
    /*!
      If handle referes to some resource it automatically releases reference to it.
      */
    ~Handle();

    //! Initializes newly created handle
    /*!
      Assigns unique magic number and index to handle, only if handle was invalid, otherwise will rise assert.
      \param index index to resource from resource manager
      */
    void Init(U16 index);

    //! Returns index of handle
    U16 GetIndex() const {return index;}

    //! Returns magic number of handle
    U16 GetMagic() const {return magic;}

    //! Returns index and magic handle as one 32-bit integer
    U32 GetHandle() const {return handle;}

    //! Returns true if magic=0 - handle is invalid
    bool IsNull() const {return !magic;}

    //! Returns reference count of handled resource
    U16 GetReferenceCount() const {return manager->GetHandleRefCount(*this);}

    operator U32() const {return handle;}

    //! Assign operator, do the same as copy constructor
    Handle<BASEDATA, FACTORY, TAG>& operator = (const Handle<BASEDATA, FACTORY, TAG>& pOther);
    bool operator == (const Handle<BASEDATA, FACTORY, TAG>& pOther) const;
    bool operator != (const Handle<BASEDATA, FACTORY, TAG>& pOther) const;

    //! Returns raw pointer to resource
    BASEDATA* Get();
    //! Returns raw const pointer to resource
    const BASEDATA* Get() const;

    static HandleManager<BASEDATA, FACTORY, TAG>* manager;

    friend class HandleManager<BASEDATA, FACTORY, TAG>;
};

template <typename BASEDATA, typename FACTORY, typename TAG>
void Handle<BASEDATA, FACTORY, TAG>::Init(U16 index) {
    ASSERT(IsNull(), "Handle already initialized");
    ASSERT(index <= MAX_INDEX, "Invalid handle index");

    //magic 0 is reserved to invalid handle
    static U16 autoMagic = 0;

    if(++autoMagic>MAX_MAGIC) {
        //assign next magic
        autoMagic=1;
    }

    this->index = index;
    this->magic = autoMagic;
}

template <typename BASEDATA, typename FACTORY, typename TAG>
inline Handle<BASEDATA, FACTORY, TAG>& Handle<BASEDATA, FACTORY, TAG>::operator = (const Handle<BASEDATA, FACTORY, TAG>& pOther) {
    Logger::Log(0, "assigning handle");

    //decrement reference count of old resource
    manager->DecrementHandleRefCount(*this);

    //copy index and magic number
    handle = pOther.handle;

    //increase reference count of new resource
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
