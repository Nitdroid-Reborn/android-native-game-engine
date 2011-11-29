#include "GameObjectManager.h"
#include <Utils/Log.h>
#include <Utils/Assert.h>
#include <Scripts/ScriptManager.h>
#include <Math/MathLib.h>
#include "RenderableGameObject.h"
#include "PhysicalGameObject.h"

GameObjectManager* GameObjectManager::singleton=NULL;
GameObjectType GameObject::type;

GameObjectManager::GameObjectManager()
{
}

void GameObjectManager::Release() {
    std::map<U32, GameObject*>::iterator it;

    for(it = objects.begin(); it != objects.end(); ++it) {
        delete (*it).second;
    }

    objects.clear();

    singleton = NULL;
    Logger::Log(1, "GameObjectManager released");
}

bool GameObjectManager::Initialize() {
    ASSERT(!singleton, "Game Object Manager already initialized");

    singleton = this;

    ScriptManager* manager = ScriptManager::Get();
    lua_State* L = manager->getState();

    luabind::module(L)
    [
        luabind::class_<Hash>("Hash")
            .def(luabind::constructor<U32>())
            .def(luabind::constructor<const char*>())
            .def(luabind::constructor<const Hash&>())
    ];

    luabind::module(L)
    [
        luabind::class_<GameObject>("GameObject")
            .def("GetId", &GameObject::GetId)
            .def("GetType", &GameObject::GetType)
            .def("GetPosition", &GameObject::GetPosition)
            .def("GetOrientation", &GameObject::GetOrientation)
            .def("GetScale", &GameObject::GetScale)
            .def("SetPosition", &GameObject::SetPosition)
            .def("SetOrientation", &GameObject::SetOrientation)
            .def("SetScale", &GameObject::SetScale)
    ];

    luabind::module(L)
    [
        luabind::class_<RenderableGameObject, GameObject>("RenderableGameObject")
            .def(luabind::constructor<Hash, const Vector3&, const Vector3&, const Vector3&, std::string, std::string, bool>())
            .def("SetVisible", &RenderableGameObject::SetVisible)
            .def("GetVisible", &RenderableGameObject::GetVisible)
            .def("GetShaderProgram", &RenderableGameObject::GetShaderProgram)
            .def("SetShaderProgram", &RenderableGameObject::SetShaderProgram)
            .def("GetModel", &RenderableGameObject::GetModel)
            .def("SetModel", &RenderableGameObject::SetModel)
            .def("SetShaderProgramParameter", (void (RenderableGameObject::*)(std::string name, const Vector2&))&RenderableGameObject::SetShaderProgramParameter)
            .def("SetShaderProgramParameter", (void (RenderableGameObject::*)(std::string name, const Vector3&))&RenderableGameObject::SetShaderProgramParameter)
            .def("SetShaderProgramParameter", (void (RenderableGameObject::*)(std::string name, const Vector4&))&RenderableGameObject::SetShaderProgramParameter)
            .def("SetShaderProgramParameter", (void (RenderableGameObject::*)(std::string name, const Matrix4x4&))&RenderableGameObject::SetShaderProgramParameter)
            .def("SetShaderProgramParameter", (void (RenderableGameObject::*)(std::string name, const F32))&RenderableGameObject::SetShaderProgramParameter)
            .def("SetShaderProgramParameter", (void (RenderableGameObject::*)(std::string name, const I32))&RenderableGameObject::SetShaderProgramParameter)
            .def("ClearShaderParameters", &RenderableGameObject::ClearShaderParameters)
    ];

    luabind::module(L)
    [
        luabind::class_<PhysicalGameObject, RenderableGameObject>("PhysicalGameObject")
            .def(luabind::constructor<Hash, const Vector3&, const Vector3&, const Vector3&, std::string, std::string, bool>())
            .def("Collide", &PhysicalGameObject::Collide)
    ];



    luabind::module(L)
    [
        luabind::class_<GameObjectManager>("GameObjectManager")
            .def("AddObject", &GameObjectManager::AddObject, luabind::adopt(_2))
            .def("FindObject", (GameObject* (GameObjectManager::*)(const Hash&))&GameObjectManager::FindObject)
            .def("FindObject", (GameObject* (GameObjectManager::*)(const char*))&GameObjectManager::FindObject)
            .def("DestroyObject", &GameObjectManager::DestroyObject)
            .scope
            [
                luabind::def("Get", &GameObjectManager::get)
            ]
    ];


    Logger::Log(1, "GameObjectManager initialized");
}

GameObjectManager::~GameObjectManager() {

}

Hash GameObjectManager::AddObject(GameObject* object) {
    ASSERT(objects[object->GetId().GetValue()]==0, "Game Object with this ID already existing");

    objects[object->GetId().GetValue()]=object;
    return object->GetId();
}


GameObject* GameObjectManager::FindObject(const Hash &id) {
    if(objects.count(id.GetValue())!=0)
        return objects.at(id.GetValue());
    return 0;
}

GameObject* GameObjectManager::FindObject(const char* id_string) {
    Hash id(id_string);
    if(objects.count(id.GetValue())!=0)
        return objects.at(id.GetValue());
    return 0;
}


/*template<class TYPE>
std::vector<TYPE*> GameObjectManager::FindObjects() {
    GameObjectType type = TYPE::GetType();

    std::vector<GameObject*> result;
    std::map<U32, GameObject*>::iterator it;
    for(it = objects.begin(); it != objects.end(); ++it) {
        if((*it).second->GetType()==type)
            result.push_back((TYPE*)(*it).second);
    }

    return result;
}*/

void GameObjectManager::DestroyObject(const Hash &id) {
    if(objects.count(id.GetValue())>0) {
        GameObject* o = objects.at(id.GetValue());
        objects.erase(id.GetValue());
        delete o;
    }
}

void GameObjectManager::Update(float dt) {
    std::map<U32, GameObject*>::iterator it;

    for(it = objects.begin(); it != objects.end(); ++it) {
        (*it).second->Update(dt);
    }
}
