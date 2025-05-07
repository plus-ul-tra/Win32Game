#include "MyFirstWndGame.h"
#include "GameTimer.h"
#include "Collider.h"
#include "GameObject.h"
#include "RenderHelp.h"
#include "TitleScene.h" // 20250421
#include "PlayScene.h"
#include "EndingScene.h"
#include <iostream>
#include <assert.h>

using namespace learning;

//constexpr int MAX_GAME_OBJECT_COUNT = 1000;

//게임에 필요한 리소스 준비
bool MyFirstWndGame::Initialize()  
{
    m_pGameTimer = new GameTimer();
    m_pGameTimer->Reset();
    
    const wchar_t* className = L"MyFirstWndGame";
    const wchar_t* windowName = L"Scene Start";

    if (false == __super::Create(className, windowName, 1000, 800)) // 창 크기 조절 1536, 995
    {
        return false;
    }


    RECT rcClient = {};
    GetClientRect(m_hWnd, &rcClient);
    m_width = rcClient.right - rcClient.left;
    m_height = rcClient.bottom - rcClient.top;

    m_hFrontDC = GetDC(m_hWnd);
    m_hBackDC = CreateCompatibleDC(m_hFrontDC);
    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    m_hDefaultBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

#pragma region resource
    //Player Bitmap
    m_pPlayer1BitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/pika.png");
    m_pPlayer2BitmapInfo = renderHelp::CreateFlippedBitmap(m_pPlayer1BitmapInfo);
    //Ball
    m_pBallBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/ball.png");
    //UI
    m_pBackgroundBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/RealTitle.png");
    m_pKeyGuideBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/ButtonGuide.png");
    m_pButtonBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/Button.png");
    m_pPlayBackgroundBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/playground2.png");
    m_pScoreBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/Score1.png");
    m_pWinUi = renderHelp::CreateBitmapInfo(L"./Resource/Win.png");
    m_pRestart = renderHelp::CreateBitmapInfo(L"./Resource/Retry.png");


    //
    if (m_pPlayer1BitmapInfo == nullptr || m_pBallBitmapInfo == nullptr
                                       || m_pBackgroundBitmapInfo == nullptr)
    {
        std::cout << "Bitmap Load Failed!" << std::endl;
        return false;
    }


#pragma endregion

    m_pScenes[SceneType::SCENE_TITLE] = new TitleScene();
    m_pScenes[SceneType::SCENE_TITLE]->Initialize(this);


    m_pScenes[SceneType::SCENE_PLAY] = new PlayScene();
    m_pScenes[SceneType::SCENE_PLAY]->Initialize(this);

    m_pScenes[SceneType::SCENE_ENDING] = new EndingScene();
    m_pScenes[SceneType::SCENE_ENDING]->Initialize(this);

    return true;
    
}

void MyFirstWndGame::Run() //key Setting
{ 
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_LBUTTONDOWN)
            {
                MyFirstWndGame::OnLButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else if (msg.message == WM_RBUTTONDOWN)
            {
                MyFirstWndGame::OnRButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else if (msg.message == WM_MOUSEMOVE)
            {
                MyFirstWndGame::OnMouseMove(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            Update(); //상태 update
            Render(); //그림
        }
    }
}

void MyFirstWndGame::Finalize()
{
    delete m_pGameTimer;
    m_pGameTimer = nullptr;

    for (int i = 0; i < SceneType::SCENE_MAX; ++i)
    {
        if (m_pScenes[i])
        {
            m_pScenes[i]->Finalize();
            delete m_pScenes[i];
            m_pScenes[i] = nullptr;
        }
    }
    __super::Destroy();

}

void MyFirstWndGame::ChangeScene(SceneType eSceneType) //Change Scene
{
    if (m_eCurrentScene != eSceneType)
    {
        m_pScenes[m_eCurrentScene]->Leave();
        m_eCurrentScene = eSceneType;
        m_pScenes[m_eCurrentScene]->Enter();
    }
}

void MyFirstWndGame::FixedUpdate()
{
    m_pScenes[m_eCurrentScene]->FixedUpdate();
 
}

void MyFirstWndGame::LogicUpdate()
{
    m_pScenes[m_eCurrentScene]->Update(m_fDeltaTime);
  
}


void MyFirstWndGame::Update()
{
    m_pGameTimer->Tick();

    LogicUpdate(); //

    m_fDeltaTime = m_pGameTimer->DeltaTimeMS();
    m_fFrameCount += m_fDeltaTime;

    while (m_fFrameCount >= 200.0f)
    {
        FixedUpdate(); //physics
        m_fFrameCount -= 200.0f;
    }
}

void MyFirstWndGame::Render()
{
    //Clear the back buffer
    ::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);
    m_pScenes[m_eCurrentScene]->Render(m_hBackDC);
    BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);

}

void MyFirstWndGame::OnResize(int width, int height)
{
    std::cout << __FUNCTION__ << std::endl;

    learning::SetScreenSize(width, height);

    __super::OnResize(width, height);

    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    HANDLE hPrevBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

    DeleteObject(hPrevBitmap);
}

void MyFirstWndGame::OnClose()
{
    std::cout << __FUNCTION__ << std::endl;

    SelectObject(m_hBackDC, m_hDefaultBitmap);

    DeleteObject(m_hBackBitmap);
    DeleteDC(m_hBackDC);

    ReleaseDC(m_hWnd, m_hFrontDC);
}

void MyFirstWndGame::OnMouseMove(int x, int y)
{
 /*   std::cout << __FUNCTION__ << std::endl;   
    std::cout << "x: " << x << ", y: " << y << std::endl;*/
    /*m_MousePosPrev = m_MousePos;
    m_MousePos = { x, y };*/
}

void MyFirstWndGame::OnLButtonDown(int x, int y)
{
      std::cout << __FUNCTION__ << std::endl;
 std::cout << "x: " << x << ", y: " << y << std::endl;
 m_EnemySpawnPos.x = x;
 m_EnemySpawnPos.y = y;
 
}

void MyFirstWndGame::OnRButtonDown(int x, int y)
{
    /*  std::cout << __FUNCTION__ << std::endl;
   std::cout << "x: " << x << ", y: " << y << std::endl;*/

    
}
