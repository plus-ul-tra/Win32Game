#include "TitleScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include<iostream>
#include <assert.h>

using namespace learning;

void TitleScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
    assert(m_pGame != nullptr && "Game object is not initialized!");

    Background* pNewObject = new Background(ObjectType::BACKGROUND);
    Ui* buttonGuide = new Ui(ObjectType::UI);
    Ui* button = new Ui(ObjectType::UI);

    pNewObject->SetPosition(0.0f, 0.0f);
    buttonGuide->SetPosition(500.0f, 400.0f);
    button->SetPosition(500.0f, 600.0f);

    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();

    pNewObject->SetWidth(width);
    pNewObject->SetHeight(height);

    buttonGuide->SetWidth(362);
    buttonGuide->SetHeight(349);
    buttonGuide->SetScale(1.0f, 1.0f);
    button->SetWidth(1024);
    button->SetHeight(1024);
    button->SetScale(0.2f, 0.2f);

    pNewObject->SetBitmapInfo(m_pGame->GetBackgroundBitmapInfo());
    buttonGuide->SetBitmapInfo(m_pGame->GetKeyGuideBitmapInfo());
    button->SetBitmapInfo(m_pGame->GetButtonBitmapInfo());

    m_rect.left = width/2 - 50;
    m_rect.top = height/2 - 50;
    m_rect.right = m_rect.left + 100;
    m_rect.bottom = m_rect.top + 100;

    m_pBackground = pNewObject;
    m_pKeyGuide = buttonGuide;
    m_pButton = button;
}

void TitleScene::Update(float deltaTime) //Scene Update
{
   static float time = 0.0f;
    time += deltaTime;
    //클릭시 Change Scene
    mousepos = m_pGame->EnemySpawnPosition();
    if (mousepos.x > 422 && mousepos.x < 578 && mousepos.y>521 && mousepos.y < 586) {
        
            //time = 0.0f;
        m_pGame->ChangeScene(SceneType::SCENE_PLAY); //ChangeScene(SceneType)
        
    }
    else
    {
        wsprintf(m_szTitle, L"Title Scene %d", static_cast<int>(time / 1000.0f) + 1);
    }
}

void TitleScene::Render(HDC hDC)
{   
    assert(m_pGame != nullptr && "Game object is not initialized!");
   
    m_pBackground->Render(hDC);
   
    m_pButton->Render(hDC);
    mousepos = m_pGame->EnemySpawnPosition();
    //click시 Render
    if (mousepos.x>420 && mousepos.x < 580 && mousepos.y>615 && mousepos.y<677) {
        if (!m_isShowGuide) {
            m_isShowGuide = true;
        }
        else{ m_isShowGuide = false; }
    }
    m_pGame->ResetEnemySpawnPosition();
    if (m_isShowGuide) {
        m_pKeyGuide->Render(hDC);
    }
    
    
    //DrawText(hDC, m_szTitle, -1, &m_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void TitleScene::Finalize()
{
        //delete m_pBackground;
        delete m_pKeyGuide;
        delete m_pButton;
        //m_pBackground = nullptr;
        m_pKeyGuide = nullptr;
        m_pButton = nullptr;
}

void TitleScene::Enter()
{
    m_pGame->ResetEnemySpawnPosition();
}

void TitleScene::Leave()
{
    std::cout << "Title Leave 호출" << std::endl;
}

