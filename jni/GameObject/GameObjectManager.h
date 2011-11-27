#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H
#include "GameObject.h"
#include <map>

class GameObjectManager
{
public:
    GameObjectManager();
    ~GameObjectManager();

    bool Initialize();
    void Release();

    Hash AddObject(GameObject *object);
    GameObject* FindObject(const Hash& id);
    GameObject* FindObject(const char* id_string);


    template<typename TYPE>
    TYPE* FindObject(const Hash &id) {
        if(objects.count(id.GetValue())!=0)
            return (TYPE*)objects.at(id.GetValue());
        return 0;
    }

   // std::vector<TYPE*> FindObjects();
    void DestroyObject(const Hash& id);

    void Update(float dt);

    static GameObjectManager* get(){return singleton;}

protected:
    static GameObjectManager* singleton;

private:
    std::map<U32, GameObject*> objects;
};

#endif // GAMEOBJECTMANAGER_H
