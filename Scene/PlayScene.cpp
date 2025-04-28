#include "PlayScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include <iostream>

using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

void PlayScene::Initialize(NzWndBase* pWnd)  //객체 생성및 전반 관리
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

    Vector2f enemyPos = m_pGame->EnemySpawnPosition();
    if (enemyPos.x != 0 && enemyPos.y != 0)
    {
        // 사실 큐가 너무 쓰고 싶었으나 참았음
        CreateEnemy();
        m_pGame->ResetEnemySpawnPosition();
    }
}

void PlayScene::Update(float deltaTime)
{
    UpdatePlayerInfo();

    UpdateEnemyInfo();

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
            m_GameObjectPtrTable[i]->Render(hDC); //Object 들은 자신을 그릴 수 있고, 여기서 일괄로 그린다.
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
{
    // [CHECK]. 첫 번째 게임 오브젝트는 플레이어 캐릭터로 고정!
    CreatePlayer(0); //1P
    CreatePlayer(1); //2P
    std::cout << "Player Created" << std::endl;

}

void PlayScene::Leave()
{
}

void PlayScene::CreatePlayer(int num)
{   //
    assert(num != 0 || num != 1 && "Player object already exists!");
    //assert(m_pGame != nullptr && "Game object is not initialized!");

    GameObject* pNewObject = new GameObject(ObjectType::PLAYER);

    if (num == 0) {
        pNewObject->SetName("Player1");
        pNewObject->SetPosition(500.0f, 900.0f);  // 하드 코딩 말고 비율로 추후수정
    }
    else if (num == 1) {
        pNewObject->SetName("Player2");
        pNewObject->SetPosition(1000.0f, 900.0f);
    }

    pNewObject->SetSpeed(0.5f); // 임의로 설정  
    pNewObject->SetWidth(100); // 임의로 설정
    pNewObject->SetHeight(100); // 임의로 설정

    pNewObject->SetBitmapInfo(m_pGame->GetPlayerBitmapInfo());
    pNewObject->SetColliderCircle(50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.

    m_GameObjectPtrTable[num] = pNewObject;


    //2P설정
}

void PlayScene::CreateEnemy()
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    GameObject* pNewObject = new GameObject(ObjectType::ENEMY);

    pNewObject->SetName("Enemy");

    Vector2f enemyPos = m_pGame->EnemySpawnPosition();

    pNewObject->SetPosition(enemyPos.x, enemyPos.y);

    pNewObject->SetSpeed(0.1f); // 일단, 임의로 설정  
    pNewObject->SetWidth(100); // 일단, 임의로 설정
    pNewObject->SetHeight(100); // 일단, 임의로 설정

    
    pNewObject->SetBitmapInfo(m_pGame->GetEnemyBitmapInfo()); //여기

    pNewObject->SetColliderCircle(50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.

    int i = 0;
    while (++i < MAX_GAME_OBJECT_COUNT) //0번째는 언제나 플레이어!
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

GameObject* PlayScene::GetPlayer(int num) const
{
    return (GameObject*)m_GameObjectPtrTable[num];
}

void PlayScene::UpdatePlayerInfo()
{
    static GameObject* pPlayer1 = GetPlayer(0);
    static GameObject* pPlayer2 = GetPlayer(1); // 추가

    assert(pPlayer1 != nullptr);
    assert(m_pGame != nullptr && "MyFirstWndGame is null!");

    Vector2f targetPos = m_pGame->PlayerTargetPosition();
    Vector2f playerPos = pPlayer1->GetPosition();

    Vector2f playerDir = targetPos - playerPos;
    float distance = playerDir.Length(); // 거리 계산

    if (distance > 50.f) //임의로 설정한 거리
    {
        playerDir.Normalize(); // 정규화
        pPlayer1->SetDirection(playerDir); // 플레이어 방향 설정
    }
    else
    {
        pPlayer1->SetDirection(Vector2f(0, 0)); // 플레이어 정지
    }
}

void PlayScene::UpdateEnemyInfo() //수정
{
    static GameObject* pPlayer = GetPlayer(0);
    assert(pPlayer != nullptr);

    Vector2f playerPos = GetPlayer(0)->GetPosition();
    for (int i = 1; i < MAX_GAME_OBJECT_COUNT; ++i) //0번째는 언제나 플레이어!
    {
        if (m_GameObjectPtrTable[i] != nullptr)
        {
            GameObject* pEnemy = static_cast<GameObject*>(m_GameObjectPtrTable[i]);
            
            Vector2f enemyPos = pEnemy->GetPosition();
            
            Vector2f enemyDir = playerPos - enemyPos;
            float distance = enemyDir.Length(); // 거리 계산

            if (distance > 50.f) //임의로 설정한 거리
            {
                enemyDir.Normalize(); // 정규화
                pEnemy->SetDirection(enemyDir); // 방향 설정
            }
            else
            {
                pEnemy->SetDirection(Vector2f(0, 0)); //  정지
            }
        }
    }
}



