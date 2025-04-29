#pragma once
#include "Scene.h"
class GameObjectBase;
class GameObject;
class MyFirstWndGame;

class PlayScene :public Scene
{
 public:
    PlayScene() = default;
    ~PlayScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override;
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;
  
private:
    void CreatePlayer(int num);
    void CreateBall();
    void UpdatePlayerInfo();
    void UpdateBallInfo();

    GameObjectBase* m_pBackground = nullptr;
    GameObject* GetPlayer(int num) const { return(GameObject*)m_GameObjectPtrTable[num]; }
    MyFirstWndGame* m_pGame = nullptr;
    RECT m_rect = { 0, 0, 0, 0 };
};

