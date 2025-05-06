#include "EndingScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include<iostream>
#include <assert.h>

void EndingScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
    assert(m_pGame != nullptr && "Game object is not initialized!");
    Ui* restartButton = new Ui(ObjectType::UI);
    //ScoreBoard* winText = new ScoreBoard(ObjectType::UI);

    restartButton->SetPosition(500.0f, 600.0f);
    restartButton->SetWidth(1024);
    restartButton->SetHeight(1024);
    restartButton->SetScale(0.2f, 0.2f);
    restartButton->SetBitmapInfo(m_pGame->GetRestartBitmapInfo());

    //winText->SetWidth(961);
    //winText->SetHeight(781);
    //winText->SetBitmapInfo(m_pGame->GetWinBitmapInfo(), 1, 2);
    //winText->SetPosition(0.0f, 50.0f);
    //winText->SetScale(1.0f, 0.5f);
    m_pButton = restartButton;
    //m_pWinBoard = winText;

    
}

void EndingScene::Update(float deltaTime)
{
    
    //std::cout << "end" << m_pGame->m_winner << std::endl;
    mousepos = m_pGame->EnemySpawnPosition();
    if (mousepos.x > 421 && mousepos.x < 579 && mousepos.y>560 && mousepos.y < 641) {
        std::cout << "re button";
        //time = 0.0f;
        m_pGame->ChangeScene(SceneType::SCENE_TITLE); //ChangeScene(SceneType)

    }
}

void EndingScene::Render(HDC hDC)
{
    m_pButton->Render(hDC);
    m_pWinBoard->Render(hDC);
}

void EndingScene::Finalize()
{
    //delete m_pBackground;
    delete m_pWinBoard;
    delete m_pButton;
    m_pWinBoard = nullptr;
    m_pButton = nullptr;
}

void EndingScene::Enter()
{
    
    if (!m_pWinBoard) {
        ScoreBoard* winText = new ScoreBoard(ObjectType::UI);
        winText->SetWidth(961);
        winText->SetHeight(781);
        winText->SetBitmapInfo(m_pGame->GetWinBitmapInfo(), 1, 2);
        winText->SetPosition(0.0f, 50.0f);
        winText->SetScale(1.0f, 0.5f);
        winText->UpdateFrame(m_pGame->m_winner);
        m_pGame->ResetEnemySpawnPosition();
        m_pWinBoard = winText;
        
    }
    
    
}

void EndingScene::Leave()
{
    delete m_pWinBoard;
    m_pWinBoard = nullptr;
}

