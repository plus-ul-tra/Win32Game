#include "PlayScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include <iostream>

using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

void PlayScene::Initialize(NzWndBase* pWnd)  //��ü ������ ���� ����
{
    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
    assert(m_pGame != nullptr);

    m_GameObjectPtrTable = new GameObjectBase*[MAX_GAME_OBJECT_COUNT];

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
        // ��� ť�� �ʹ� ���� �;����� ������
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

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Render(hDC); //Object ���� �ڽ��� �׸� �� �ְ�, ���⼭ �ϰ��� �׸���.
        }
    }
}

void PlayScene::Finalize()
{
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
    // [CHECK]. ù ��° ���� ������Ʈ�� �÷��̾� ĳ���ͷ� ����!
    CreatePlayer(0); //1P
    CreatePlayer(1); //2P
    std::cout << "Player Created" << std::endl;

}

void PlayScene::Leave()
{
}

void PlayScene::CreatePlayer(int num)
{
    assert(num != 0 || num != 1 && "Player object already exists!");
    //assert(m_pGame != nullptr && "Game object is not initialized!");

    GameObject* pNewObject = new GameObject(ObjectType::PLAYER);

    if (num == 0) {
        pNewObject->SetName("Player1");
        pNewObject->SetPosition(500.0f, 900.0f);  // �ϵ� �ڵ� ���� ������ ���ļ���
    }
    else if (num == 1) {
        pNewObject->SetName("Player2");
        pNewObject->SetPosition(1000.0f, 900.0f);
    }

    pNewObject->SetSpeed(0.5f); // �ϴ�, ���Ƿ� ����  
    pNewObject->SetWidth(150); // �ϴ�, ���Ƿ� ����
    pNewObject->SetHeight(150); // �ϴ�, ���Ƿ� ����

    pNewObject->SetBitmapInfo(m_pGame->GetPlayerBitmapInfo());
    pNewObject->SetColliderCircle(50.0f); // �ϴ�, ���Ƿ� ����. ������Ʈ ������ �� �� �ϰ� ���� ����.

    m_GameObjectPtrTable[num] = pNewObject;


    //2P����
}

void PlayScene::CreateEnemy()
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    GameObject* pNewObject = new GameObject(ObjectType::ENEMY);

    pNewObject->SetName("Enemy");

    Vector2f enemyPos = m_pGame->EnemySpawnPosition();

    pNewObject->SetPosition(enemyPos.x, enemyPos.y);

    pNewObject->SetSpeed(0.1f); // �ϴ�, ���Ƿ� ����  
    pNewObject->SetWidth(100); // �ϴ�, ���Ƿ� ����
    pNewObject->SetHeight(100); // �ϴ�, ���Ƿ� ����

    
    pNewObject->SetBitmapInfo(m_pGame->GetEnemyBitmapInfo()); //����

    pNewObject->SetColliderCircle(50.0f); // �ϴ�, ���Ƿ� ����. ������Ʈ ������ �� �� �ϰ� ���� ����.

    int i = 0;
    while (++i < MAX_GAME_OBJECT_COUNT) //0��°�� ������ �÷��̾�!
    {
        if (nullptr == m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i] = pNewObject;
            break;
        }
    }

    if (i == MAX_GAME_OBJECT_COUNT)
    {
        // ���� ������Ʈ ���̺��� ���� á���ϴ�.
        delete pNewObject;
        pNewObject = nullptr;
    }
}

void PlayScene::UpdatePlayerInfo()
{
    static GameObject* pPlayer = GetPlayer();

    assert(pPlayer != nullptr);
    assert(m_pGame != nullptr && "MyFirstWndGame is null!");

    Vector2f targetPos = m_pGame->PlayerTargetPosition();
    Vector2f playerPos = pPlayer->GetPosition();

    Vector2f playerDir = targetPos - playerPos;
    float distance = playerDir.Length(); // �Ÿ� ���

    if (distance > 50.f) //���Ƿ� ������ �Ÿ�
    {
        playerDir.Normalize(); // ����ȭ
        pPlayer->SetDirection(playerDir); // �÷��̾� ���� ����
    }
    else
    {
        pPlayer->SetDirection(Vector2f(0, 0)); // �÷��̾� ����
    }
}

void PlayScene::UpdateEnemyInfo()
{
    static GameObject* pPlayer = GetPlayer();
    assert(pPlayer != nullptr);

    Vector2f playerPos = GetPlayer()->GetPosition();
    for (int i = 1; i < MAX_GAME_OBJECT_COUNT; ++i) //0��°�� ������ �÷��̾�!
    {
        if (m_GameObjectPtrTable[i] != nullptr)
        {
            GameObject* pEnemy = static_cast<GameObject*>(m_GameObjectPtrTable[i]);
            
            Vector2f enemyPos = pEnemy->GetPosition();
            
            Vector2f enemyDir = playerPos - enemyPos;
            float distance = enemyDir.Length(); // �Ÿ� ���

            if (distance > 50.f) //���Ƿ� ������ �Ÿ�
            {
                enemyDir.Normalize(); // ����ȭ
                pEnemy->SetDirection(enemyDir); // ���� ����
            }
            else
            {
                pEnemy->SetDirection(Vector2f(0, 0)); //  ����
            }
        }
    }
}

