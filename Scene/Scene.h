#pragma once
#include "INC_Windows.h"
#include<vector>
#include "GameObject.h"

enum SceneType
{
    SCENE_TITLE = 0,
    SCENE_PLAY,
    SCENE_ENDING,
    SCENE_MAX
};

class GameObjectBase;
class NzWndBase;

class Scene
{
public:
    Scene() = default;
    virtual ~Scene() = default;
    virtual void Initialize(NzWndBase* pWnd) = 0;
    virtual void Finalize() = 0;

    virtual void Enter() = 0;
    virtual void Leave() = 0;

    virtual void FixedUpdate() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(HDC hDC) = 0;
    void AddObject(GameObjectBase* pObj) {
        m_objects.push_back(pObj);
    }
    

protected:
    GameObjectBase** m_GameObjectPtrTable = nullptr;
    std::vector<GameObjectBase*> m_objects;

private:
    Scene (const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
};


