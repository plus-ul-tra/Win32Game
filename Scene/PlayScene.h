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
    void CreateNet();
    void UpdatePlayerInfo();
    //void UpdateBallInfo();
    

    GameObjectBase* m_pBackground = nullptr;
    Player* GetPlayer(int num) const { return(Player*)m_GameObjectPtrTable[num]; }  //downcast;
    Ball* GetBall() const { return(Ball*)m_GameObjectPtrTable[2]; }
    Net* GetNet() const { return(Net*)m_GameObjectPtrTable[3]; }
    MyFirstWndGame* m_pGame = nullptr;
    RECT m_rect = { 0, 0, 0, 0 };
};

