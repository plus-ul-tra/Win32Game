#pragma once
#include "Scene.h"

class MyFirstWndGame;

class EndingScene : public Scene
{
public:
    EndingScene() = default;
    ~EndingScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override {}
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;
private:
    learning::Vector2f mousepos = { 0,0 };
    MyFirstWndGame* m_pGame = nullptr;
    GameObjectBase* m_pButton = nullptr;
    GameObjectBase* m_pWinBoard = nullptr;
};

