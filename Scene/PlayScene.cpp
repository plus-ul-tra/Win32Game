#include "PlayScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include <iostream>

using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 10;

void PlayScene::Initialize(NzWndBase* pWnd)  
{
    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
    assert(m_pGame != nullptr);

    m_GameObjectPtrTable = new GameObjectBase*[MAX_GAME_OBJECT_COUNT];
    Background* pNewObject = new Background(ObjectType::BACKGROUND);
    pNewObject->SetPosition(0.0f, 0.0f);
    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();
    pNewObject->SetWidth(width);
    pNewObject->SetHeight(height);

    pNewObject->SetBitmapInfo(m_pGame->GetPlayBackgroundBitmapInfo());

    m_rect.left = width / 2 - 50;
    m_rect.top = height / 2 - 50;
    m_rect.right = m_rect.left + 100;
    m_rect.bottom = m_rect.top + 100;
    m_pBackground = pNewObject;

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        m_GameObjectPtrTable[i] = nullptr;
    }

}

void PlayScene::FixedUpdate()
{
    assert(m_pGame != nullptr && "Game object is not initialized!");
    
        //물리, 물리 판정
       
    
}

void PlayScene::Update(float deltaTime)
{
    UpdatePlayerInfo();
    //UpdateBallInfo();
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Update(deltaTime);
        }
    }
}

void PlayScene::Render(HDC hDC)
{
    assert(m_pGame != nullptr && "Game object is not initialized!");
    m_pBackground->Render(hDC);
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Render(hDC); //<<
        }
    }
}

void PlayScene::Finalize()
{
    if (m_pBackground)
    {
        delete m_pBackground;
        m_pBackground = nullptr;
    }
   if (m_GameObjectPtrTable)
   {
       for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
       {
           if (m_GameObjectPtrTable[i])
           {
               delete m_GameObjectPtrTable[i];
               m_GameObjectPtrTable[i] = nullptr;
           }
       }

      delete [] m_GameObjectPtrTable;
   }
}

void PlayScene::Enter()
{   //처음 Scene 진입, 점수 득점 시 수정
    // [CHECK]. 첫 번째 게임 오브젝트는 플레이어 캐릭터로 고정!
    CreatePlayer(0); //1P
    CreatePlayer(1); //2P
    CreateBall(); //index로 공 위치 조정
    std::cout << "Player Created" << std::endl;

}

void PlayScene::Leave()
{
}

void PlayScene::CreatePlayer(int num)
{   //
    assert(num != 0 || num != 1 && "Player object already exists!");
    //assert(m_pGame != nullptr && "Game object is not initialized!");

    //GameObject* pNewObject = new GameObject(ObjectType::PLAYER);
    Player* pNewObject = new Player(ObjectType::PLAYER);
    pNewObject->SetSpeed(0.35f,0.5f);   
    pNewObject->SetWidth(150); 
    pNewObject->SetHeight(150); 
    // 각각 영역으로 주기
    pNewObject->SetBoundaryInfo(m_pGame->GetWidth()+10, m_pGame->GetHeight());

    if (num == 0) {
        pNewObject->SetName("Player1");
        pNewObject->SetPosition(m_pGame->GetWidth()/4, m_pGame->GetHeight()- pNewObject->GetHeight());  
        pNewObject->SetBitmapInfo(m_pGame->GetPlayerBitmapInfo(), 7, 4);
    }
    else if (num == 1) {
        pNewObject->SetName("Player2");
        pNewObject->SetPosition((m_pGame->GetWidth() / 4) * 3 , m_pGame->GetHeight()-pNewObject->GetHeight()); //생성 위치
        pNewObject->SetBitmapInfo(m_pGame->GetPlayer2BitmapInfo(), 7, 4);
    }

    

    pNewObject->SetColliderCircle(40.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.
    pNewObject->SetColliderBox(100.0f,100.0f);
    m_GameObjectPtrTable[num] = pNewObject;


    //2P설정
}

void PlayScene::CreateBall()
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    //GameObject* pNewObject = new GameObject(ObjectType::BAll);
    Ball* pNewObject = new Ball(ObjectType::BAll);
    pNewObject->SetName("Ball");

    //Vector2f enemyPos = m_pGame->EnemySpawnPosition();

    // Ball Spawn pos
    pNewObject->SetPosition((m_pGame->GetWidth() / 2), m_pGame->GetHeight()/2);
    pNewObject->SetSpeed(0.5f,0.5f);
    pNewObject->SetWidth(100); 
    pNewObject->SetHeight(100); 

    // boundary 제한
    pNewObject->SetBoundaryInfo(m_pGame->GetWidth(), m_pGame->GetHeight()); //흠

    pNewObject->SetBitmapInfo(m_pGame->GetBallBitmapInfo(),6,1); //여기

    pNewObject->SetColliderCircle(50.0f);

    int i = 0;
    while (++i < MAX_GAME_OBJECT_COUNT) 
    {
        if (nullptr == m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i] = pNewObject;
            break;
        }
    }

    if (i == MAX_GAME_OBJECT_COUNT)
    {
        // 게임 오브젝트 테이블이 가득 찼습니다.
        delete pNewObject;
        pNewObject = nullptr;
    }
}
void PlayScene::UpdatePlayerInfo() //physics
{
    static Player* p1 = GetPlayer(0);
    static Player* p2 = GetPlayer(1); // 추가

    assert(p1 != nullptr);
    assert(p2 != nullptr);
    assert(m_pGame != nullptr && "MyFirstWndGame is null!");

    PlayerInput input1;
    input1.moveLeft = GetAsyncKeyState('A') & 0x8000;
    input1.moveRight = GetAsyncKeyState('D') & 0x8000;
    input1.jump = GetAsyncKeyState('W') & 0x8000;

    PlayerInput input2;
    input2.moveLeft = GetAsyncKeyState(VK_LEFT) & 0x8000;
    input2.moveRight = GetAsyncKeyState(VK_RIGHT) & 0x8000;
    input2.jump = GetAsyncKeyState(VK_UP) & 0x8000;

    p1->SetInput(input1);
    p2->SetInput(input2);

}

void PlayScene::UpdateBallInfo() //수정
{
    static GameObject* pPlayer = GetPlayer(3); //3으로 수정
    assert(pPlayer != nullptr);

    // 공에 대한 물리적용
}



