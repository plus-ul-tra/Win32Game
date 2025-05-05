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
    if (m_isScore) {
        m_roundTimer -= deltaTime;
        if (m_roundTimer <= 0.0f) {
            SetNewRound(); // 공과 플레이어 위치 초기화
            m_isScore = false;
        }
        return; // 점수 후 대기 상태이므로 나머지 업데이트 생략
    }
    UpdatePlayerInfo();
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Update(deltaTime);
        }
    }
    if (m_player1Score == MAX_SCORE || m_player2Score == MAX_SCORE) {
        m_pGame->ChangeScene(SceneType::SCENE_ENDING);
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

void PlayScene::SetNewRound()
{
    static Player* p1 = GetPlayer(0);
    static Player* p2 = GetPlayer(1); // 추가
    static Ball* ball = GetBall();
    static ScoreBoard* score1 = GetScore(4);
    static ScoreBoard* score2 = GetScore(5);

        p1->SetPosition(m_pGame->GetWidth() / 7, m_pGame->GetHeight() - p1->GetHeight());
        p2->SetPosition((m_pGame->GetWidth() / 7) * 6, m_pGame->GetHeight() - p2->GetHeight());
        ball->SetSpeed(0.4f, 0.4f);
        ball->SetDirection({ 0.0f, 1.0f });
        if (m_winner == 1) {
            ball->SetPosition((m_pGame->GetWidth() / 7), m_pGame->GetHeight() / 2);
            m_player1Score++;
            score1->UpdateFrame(m_player1Score);
        }
        else if (m_winner == 2) {
            ball->SetPosition((m_pGame->GetWidth() / 7) * 6, m_pGame->GetHeight() / 2);
            m_player2Score++;
            score2->UpdateFrame(m_player2Score);
        }
        //std::cout << m_player1Score << ":" << m_player2Score << std::endl;
        m_isScore = false;
        m_roundTimer = 1000.0f;

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
{ 
    CreatePlayer(0); //1P
    CreatePlayer(1); //2P
    CreateBall(); 
    CreateNet();
}

void PlayScene::Leave()
{
    std::cout << "Leave 호출" << std::endl;
}

void PlayScene::CreatePlayer(int num)
{   //
    assert(num != 0 || num != 1 && "Player object already exists!");
    //assert(m_pGame != nullptr && "Game object is not initialized!");

    //GameObject* pNewObject = new GameObject(ObjectType::PLAYER);
    Player* pNewObject = new Player(ObjectType::PLAYER);
    ScoreBoard* pNewScore = new ScoreBoard(ObjectType::UI);
    pNewObject->SetSpeed(0.35f,0.5f);   
    pNewObject->SetWidth(150); 
    pNewObject->SetHeight(150); 
    // 각각 영역으로 주기
    //pNewObject->SetBoundaryInfo(m_pGame->GetWidth(), m_pGame->GetHeight());

    if (num == 0) {
        pNewObject->playerIndex = num;
        pNewObject->SetName("Player1");
        pNewObject->SetPosition(m_pGame->GetWidth()/7, m_pGame->GetHeight()- pNewObject->GetHeight());
        pNewObject->SetPlayerBoundaryInfo(0,(m_pGame->GetWidth()/2)-30, m_pGame->GetHeight());
        pNewObject->SetBitmapInfo(m_pGame->GetPlayerBitmapInfo(), 7, 4);

        pNewScore->SetWidth(878);
        pNewScore->SetHeight(779);
        pNewScore->SetBitmapInfo(m_pGame->GetScoreBitmapInfo(),4,3);
        pNewScore->SetPosition(0.0f,50.0f);
        pNewScore->SetScale(0.1f, 0.1f);
        m_GameObjectPtrTable[4] = pNewScore;
    }
    else if (num == 1) {
        pNewObject->playerIndex = num;
        pNewObject->SetName("Player2");
        pNewObject->SetPosition((m_pGame->GetWidth() / 7) * 6 , m_pGame->GetHeight()-pNewObject->GetHeight()); //생성 위치
        pNewObject->SetPlayerBoundaryInfo((m_pGame->GetWidth() / 2)+30, m_pGame->GetWidth(), m_pGame->GetHeight());
        pNewObject->SetBitmapInfo(m_pGame->GetPlayer2BitmapInfo(), 7, 4);

        pNewScore->SetWidth(878);
        pNewScore->SetHeight(779);
        pNewScore->SetBitmapInfo(m_pGame->GetScoreBitmapInfo(), 4, 3);
        pNewScore->SetPosition(900.0f, 50.0f);
        pNewScore->SetScale(0.1f, 0.1f);
        m_GameObjectPtrTable[5] = pNewScore;
    }

    

    pNewObject->SetColliderCircle(60.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.
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
    //static Ui* score1 = GetScore(4); //p1
    //static Ui* score2 = GetScore(5); //p2

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
                
            }
        }
    }

    p2->prevEnterPressed = currentEnterPressed;
    
    p1->SetInput(input1); //여기서 z만 따로 주기?
    p2->SetInput(input2);
    ball->CheckCollision(*p1->m_pColliderCircle, *p2->m_pColliderCircle);
    ball->CollisionNet(*net->m_pColliderBox);
    m_isScore = ball->m_isScore;
    m_winner = ball->m_winPlayer;
    ball->m_isScore = false;

}



