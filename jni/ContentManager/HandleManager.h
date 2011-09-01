#ifndef HANDLEMANAGER_H
#define HANDLEMANAGER_H
#include <Utils/Utils.h>
#include <vector>

using namespace std;


template <typename BASEDATA, typename FACTORY, typename TAG>
class Handle;


template <typename BASEDATA, typename FACTORY, typename TAG>
class HandleManager
{
    vector<BASEDATA*> data;
    vector<U16> magicNumbers;
    vector<U16> freeHandles;
    vector<U16> handlesRefCount;

    FACTORY* dataFactory;
public:
    HandleManager(){}
    ~HandleManager()
    {
        for(int i=0;i<data.size();i++) {
            if(data[i]!=NULL) {
                delete data[i];
                data[i] = NULL;
            }
        }
    }

    void SetDataFactory(FACTORY* f) {
        dataFactory = f;
    }

    BASEDATA* Acquire(Handle<BASEDATA, FACTORY, TAG>& handle);
    void Release(Handle<BASEDATA, FACTORY, TAG>&);
    BASEDATA* Dereference(const Handle<BASEDATA, FACTORY, TAG>&);
    const BASEDATA* Dereference(const Handle<BASEDATA, FACTORY, TAG>&) const;

    U32 GetUsedHandlesCount() const {
        return magicNumbers.size() - freeHandles.size();
    }

    bool HasUsedHandles() const {
        return (!GetUsedHandlesCount());
    }

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
    if(freeHandles.empty()) {
        index = magicNumbers.size();
        handle.Init(index);
        BASEDATA* newData =  dataFactory->Create();
        //newData->IncrementReferenceCount();
        data.push_back(newData);
        handlesRefCount.push_back(1);
        magicNumbers.push_back(handle.GetMagic());
    }
    else {
        index = freeHandles.back();
        handle.Init(index);
        freeHandles.pop_back();
        BASEDATA* newData =  dataFactory->Create();
       // newData->IncrementReferenceCount();
        handlesRefCount[index] = 1;
        data[index] = newData;
        magicNumbers[index] = handle.GetMagic();
    }

    Log("texture index %d", index);
    return *(data.begin() + index);
}

template <typename BASEDATA, typename FACTORY, typename TAG>
void HandleManager<BASEDATA, FACTORY, TAG>::Release(Handle<BASEDATA, FACTORY, TAG> &handle) {
    U16 index = handle.GetIndex();


    ASSERT(index < data.size(), "Invalid handle");
    ASSERT(magicNumbers[index] == handle.GetMagic(), "Invalid handle");

    handlesRefCount[index]--;

    handle.Invalidate();

    if(handlesRefCount[index]==0) {
        Log("Releasing asset");
        magicNumbers[index]=0;
        delete data[index];
        data[index]=NULL;
        freeHandles.push_back(index);
    }
}

template <typename BASEDATA, typename FACTORY, typename TAG>
inline BASEDATA* HandleManager<BASEDATA, FACTORY, TAG>::Dereference(const Handle<BASEDATA, FACTORY, TAG> &handle) {
    if(handle.IsNull())
        return NULL;

    U16 index = handle.GetIndex();
    if(index >= data.size() || magicNumbers[index]!=handle.GetMagic()) {
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
