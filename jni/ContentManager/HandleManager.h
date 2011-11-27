#ifndef HANDLEMANAGER_H
#define HANDLEMANAGER_H
#include <Utils/Utils.h>
#include <vector>

using namespace std;


template <typename BASEDATA, typename FACTORY, typename TAG>
class Handle;


//! Generic handle manager
/*!
  Manages raw resources which are referenced by Handle object.
  */
template <typename BASEDATA, typename FACTORY, typename TAG>
class HandleManager
{
    //list of raw data
    vector<BASEDATA*> data;
    //list of magic numbers of handles
    vector<U16> magicNumbers;
    //number of free handles indices
    vector<U16> freeHandles;

    //reference count of raw data
    vector<U16> handlesRefCount;

    //creates raw data
    FACTORY* dataFactory;
public:
    HandleManager(){}

    //! Destructor
    /*!
      If there are still some data releases it. It don't invalidate handles which
      references to this data!
      */
    ~HandleManager()
    {
        Logger::Log("Left %d", data.size());
        for(int i=0;i<data.size();i++) {
            if(data[i]!=NULL) {
                delete data[i];
                data[i] = NULL;
            }
        }
        data.clear();

        delete dataFactory;
    }

    void SetDataFactory(FACTORY* f) {
        dataFactory = f;
    }

    //! Acquire new resource
    /*!
      Creates new resource and assigns it to handle.
      \param handle handle which will reference to resource
      \returns Pointer to newly created resource
      */
    BASEDATA* Acquire(Handle<BASEDATA, FACTORY, TAG>& handle);

    //! Releases resource
    /*!
      Releases resource, decreases it reference count and if it drops to 0
      delete resource from memory. It invalidates handle.
      \param handle handle to release
      */
    void Release(Handle<BASEDATA, FACTORY, TAG>&handle);

    //! Returns raw data of handle
    /*!
      Checks if handle is correct and if it's index and magic numbers are the same
      as ones in manager, otherwise rise assert.
      \param handle handle of resource
      */
    BASEDATA* Dereference(const Handle<BASEDATA, FACTORY, TAG>&handle );

    //! Returns const raw data of handle
    /*!
      Checks if handle is correct and if it's index and magic numbers are the same
      as ones in manager, otherwise rise assert.
      \param handle handle to resource
      */
    const BASEDATA* Dereference(const Handle<BASEDATA, FACTORY, TAG>&handle) const;

    //! Returns number of loaded resources
    U32 GetUsedHandlesCount() const {
        return magicNumbers.size() - freeHandles.size();
    }

    //! Returns true if has any loaded resources
    bool HasUsedHandles() const {
        return (!GetUsedHandlesCount());
    }

    //! Returns reference count of resource referenced by handle
    U16 GetHandleRefCount(const Handle<BASEDATA, FACTORY, TAG>& handle) const{
        if(handle.GetMagic()!=0)
            return handlesRefCount[handle.GetIndex()];
        return 0;
    }

    void IncrementHandleRefCount(const Handle<BASEDATA, FACTORY, TAG>&);
    void DecrementHandleRefCount(const Handle<BASEDATA, FACTORY, TAG>&);
};

template <typename BASEDATA, typename FACTORY, typename TAG>
inline void HandleManager<BASEDATA, FACTORY, TAG>::IncrementHandleRefCount(const Handle<BASEDATA, FACTORY, TAG> &handle) {
    if(handle.GetMagic()!=0) {
        handlesRefCount[handle.GetIndex()]++;
    }
}

template <typename BASEDATA, typename FACTORY, typename TAG>
inline void HandleManager<BASEDATA, FACTORY, TAG>::DecrementHandleRefCount(const Handle<BASEDATA, FACTORY, TAG> &handle) {
    if(handle.GetMagic()!=0) {
        handlesRefCount[handle.GetIndex()]--;

        ASSERT(handlesRefCount[handle.GetIndex()]>=0, "Handle reference count cannot be lower than 0");
    }
}


template <typename BASEDATA, typename FACTORY, typename TAG>
BASEDATA* HandleManager<BASEDATA, FACTORY, TAG>::Acquire(Handle<BASEDATA, FACTORY, TAG> &handle) {
    U16 index;
    //if there are no free indices
    if(freeHandles.empty()) {
        //index is number of last index+1
        index = magicNumbers.size();
        handle.Init(index);
        //create new data
        BASEDATA* newData =  dataFactory->Create();
        //push it to vector of data - it will allocate new vector
        data.push_back(newData);
        //set ref count of new data to 1
        handlesRefCount.push_back(1);
        //save magic number to error checking
        magicNumbers.push_back(handle.GetMagic());
    }
    else {
        //get index of free index
        index = freeHandles.back();
        handle.Init(index);
        freeHandles.pop_back();
        BASEDATA* newData =  dataFactory->Create();
       // newData->IncrementReferenceCount();

        //set ref count of new data to 1
        handlesRefCount[index] = 1;
        //save new data on index position
        data[index] = newData;
        //save magic number
        magicNumbers[index] = handle.GetMagic();
    }

    return *(data.begin() + index);
}

template <typename BASEDATA, typename FACTORY, typename TAG>
void HandleManager<BASEDATA, FACTORY, TAG>::Release(Handle<BASEDATA, FACTORY, TAG> &handle) {
    U16 index = handle.GetIndex();

    ASSERT(index < data.size(), "Invalid handle");
    ASSERT(magicNumbers[index] == handle.GetMagic(), "Invalid handle");

    //decrese ref count
    handlesRefCount[index]--;

    //invalidate handle - set magic to reserved 0
    handle.Invalidate();

    //if ref count drops to 0 delete resource
    if(handlesRefCount[index]==0) {
        Logger::Log(0, "Releasing asset");
        magicNumbers[index]=0;
        delete data[index];
        data[index]=NULL;
        //add index to free indices list
        freeHandles.push_back(index);
    }
}

template <typename BASEDATA, typename FACTORY, typename TAG>
inline BASEDATA* HandleManager<BASEDATA, FACTORY, TAG>::Dereference(const Handle<BASEDATA, FACTORY, TAG> &handle) {
    if(handle.IsNull())
        return NULL;

    U16 index = handle.GetIndex();
    if(index >= data.size() || magicNumbers[index]!=handle.GetMagic()) {
        Logger::Log("handle index: %d, magic: %d", handle.GetIndex(), handle.GetMagic());
        ASSERT(0, "Invalid handle used");
        return 0;
    }
    return *(data.begin() + index);
}

template <typename BASEDATA, typename FACTORY, typename TAG>
inline const BASEDATA* HandleManager<BASEDATA, FACTORY, TAG>::Dereference(const Handle<BASEDATA, FACTORY, TAG> &handle) const{
    if(handle.IsNull())
        return NULL;

    U16 index = handle.GetIndex();
    if(index >= data.size() | magicNumbers[index]!=handle.GetMagic()) {
        ASSERT(0, "Invalid handle used");
        return 0;
    }
    return *(data.begin() + index);
}

#endif // HANDLEMANAGER_H
