#pragma once

#include "Scene.h"

class GameObjectBase;
class MyFirstWndGame;

class TitleScene : public Scene
{
public:
    TitleScene() = default;
    ~TitleScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override {}
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;

private:
    learning::Vector2f mousepos = {0,0};
    GameObjectBase* m_pBackground = nullptr;
    GameObjectBase* m_pStartButton = nullptr;
    GameObjectBase* m_pKeyGuideButton = nullptr;
    GameObjectBase* m_pKeyGuide = nullptr;
    GameObjectBase* m_pButton = nullptr;
    bool m_isShowGuide = false;
    MyFirstWndGame* m_pGame = nullptr;

    WCHAR m_szTitle[64] = L"Title Scene";

    RECT m_rect = { 0, 0, 0, 0 };
};

