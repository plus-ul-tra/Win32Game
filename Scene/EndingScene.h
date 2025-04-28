#pragma once
#include "Scene.h"
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

};

