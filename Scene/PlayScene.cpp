#include "PlayScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include "Collider.h"
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
    //UpdatePlayerInfo();
    //assert(m_pGame != nullptr && "Game object is not initialized!");
    //static Player* p1 = GetPlayer(0);
    //static Player* p2 = GetPlayer(1); // 추가
    //static Ball* ball = GetBall();
    //ball->CheckCollision(*p1->m_pColliderCircle, *p2->m_pColliderCircle);
    //    //물리, 물리 판정
}

void PlayScene::Update(float deltaTime)
{
    UpdatePlayerInfo();
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
    CreateNet();
    //std::cout << "Player Created" << std::endl;

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
    //pNewObject->SetBoundaryInfo(m_pGame->GetWidth(), m_pGame->GetHeight());

    if (num == 0) {
        pNewObject->SetName("Player1");
        pNewObject->SetPosition(m_pGame->GetWidth()/7, m_pGame->GetHeight()- pNewObject->GetHeight());  
        pNewObject->SetPlayerBoundaryInfo(0,(m_pGame->GetWidth()/2)-30, m_pGame->GetHeight());
        pNewObject->SetBitmapInfo(m_pGame->GetPlayerBitmapInfo(), 7, 4);
    }
    else if (num == 1) {
        pNewObject->SetName("Player2");
        pNewObject->SetPosition((m_pGame->GetWidth() / 7) * 6 , m_pGame->GetHeight()-pNewObject->GetHeight()); //생성 위치
        pNewObject->SetPlayerBoundaryInfo((m_pGame->GetWidth() / 2)+30, m_pGame->GetWidth(), m_pGame->GetHeight());
        pNewObject->SetBitmapInfo(m_pGame->GetPlayer2BitmapInfo(), 7, 4);
    }

    

    pNewObject->SetColliderCircle(40.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.
    pNewObject->SetColliderBox(120.0f,140.0f);
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
    pNewObject->SetPosition((m_pGame->GetWidth() / 7), m_pGame->GetHeight()/2);
    pNewObject->SetSpeed(0.4f,0.4f);
    pNewObject->SetDirection({ 0.0f, 1.0f });
    pNewObject->SetWidth(100); 
    pNewObject->SetHeight(100); 

    // boundary 제한
    pNewObject->SetBoundaryInfo(m_pGame->GetWidth(), m_pGame->GetHeight()); //흠

    pNewObject->SetBitmapInfo(m_pGame->GetBallBitmapInfo(),6,1); //여기

    pNewObject->SetColliderCircle(30.0f);
    pNewObject->SetColliderBox(110.0f,100.0f);

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
        delete pNewObject;
        pNewObject = nullptr;
    }
}
void PlayScene::CreateNet() {
    Net* pNewObject = new Net(ObjectType::NET);
    pNewObject->SetName("Net");
    pNewObject->SetPosition((m_pGame->GetWidth() / 2), m_pGame->GetHeight() - 100);
    pNewObject->SetColliderBox(10.0f, 350.0f);
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
        delete pNewObject;
        pNewObject = nullptr;
    }
}
void PlayScene::UpdatePlayerInfo() //physics
{
    static Player* p1 = GetPlayer(0);
    static Player* p2 = GetPlayer(1); // 추가
    static Ball* ball = GetBall();
    static Net* net = GetNet();

    assert(p1 != nullptr);
    assert(p2 != nullptr);
    assert(m_pGame != nullptr && "MyFirstWndGame is null!");
    
    PlayerInput input1;
    input1.moveLeft = GetAsyncKeyState('D') & 0x8000;
    input1.moveRight = GetAsyncKeyState('G') & 0x8000;
    input1.jump = GetAsyncKeyState('R') & 0x8000;
    bool currentZPressed = (GetAsyncKeyState('Z') & 0x8000);
    bool zDownOnce = currentZPressed && !p1->prevZPressed;

    if (zDownOnce)
    { 
        if (p1->m_isGrounded && !p1->m_isSlideOnCooldown) {
            //땅일때 Slide
            p1->m_isSkill = true;
        }
        else {
            //공중일때 Spike
            if (learning::Intersect(*p1->m_pColliderBox, *ball->m_pColliderBox)) {
                ball->m_isHit =true;
                ball->SetDirection({ 1.0f, 0.7f });
                printf("Spiked!\n");
            }
        }
    }
    p1->prevZPressed = currentZPressed;
    //input1.skill = (GetAsyncKeyState('Z') & 0x8000) != 0;
    
    PlayerInput input2;
    input2.moveLeft = GetAsyncKeyState(VK_LEFT) & 0x8000;
    input2.moveRight = GetAsyncKeyState(VK_RIGHT) & 0x8000;
    input2.jump = GetAsyncKeyState(VK_UP) & 0x8000;
    //input2.skill = GetAsyncKeyState(VK_RETURN) & 0x8000;

    bool currentEnterPressed = GetAsyncKeyState(VK_RETURN) & 0x8000;
    bool enterDownOnce = currentEnterPressed && !p2->prevEnterPressed;
    if (enterDownOnce)
    {
        if (p2->m_isGrounded && !p2->m_isSlideOnCooldown) {
            //땅일때 Slide
            p2->m_isSkill = true;
        }
        else {
            //공중일때 Spike
            if (learning::Intersect(*p2->m_pColliderBox, *ball->m_pColliderBox)) {
                ball->m_isHit = true;
                ball->SetDirection({ -1.0f,0.7f });
                printf("Spiked!\n");
            }
        }
    }

    p2->prevEnterPressed = currentEnterPressed;
    
    p1->SetInput(input1); //여기서 z만 따로 주기?
    p2->SetInput(input2);
    ball->CheckCollision(*p1->m_pColliderCircle, *p2->m_pColliderCircle);
    ball->CollisionNet(*net->m_pColliderBox);

}



