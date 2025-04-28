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
    m_pBackground->Render(hDC);
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
    // [CHECK]. ù ��° ���� ������Ʈ�� �÷��̾� ĳ���ͷ� ����!
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
        pNewObject->SetPosition(500.0f, 900.0f);  // �ϵ� �ڵ� ���� ������ ���ļ���
    }
    else if (num == 1) {
        pNewObject->SetName("Player2");
        pNewObject->SetPosition(1000.0f, 900.0f);
    }

    pNewObject->SetSpeed(0.5f); // ���Ƿ� ����  
    pNewObject->SetWidth(100); // ���Ƿ� ����
    pNewObject->SetHeight(100); // ���Ƿ� ����

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

GameObject* PlayScene::GetPlayer(int num) const
{
    return (GameObject*)m_GameObjectPtrTable[num];
}

void PlayScene::UpdatePlayerInfo()
{
    static GameObject* pPlayer1 = GetPlayer(0);
    static GameObject* pPlayer2 = GetPlayer(1); // �߰�

    assert(pPlayer1 != nullptr);
    assert(m_pGame != nullptr && "MyFirstWndGame is null!");

    Vector2f targetPos = m_pGame->PlayerTargetPosition();
    Vector2f playerPos = pPlayer1->GetPosition();

    Vector2f playerDir = targetPos - playerPos;
    float distance = playerDir.Length(); // �Ÿ� ���

    if (distance > 50.f) //���Ƿ� ������ �Ÿ�
    {
        playerDir.Normalize(); // ����ȭ
        pPlayer1->SetDirection(playerDir); // �÷��̾� ���� ����
    }
    else
    {
        pPlayer1->SetDirection(Vector2f(0, 0)); // �÷��̾� ����
    }
}

void PlayScene::UpdateEnemyInfo() //����
{
    static GameObject* pPlayer = GetPlayer(0);
    assert(pPlayer != nullptr);

    Vector2f playerPos = GetPlayer(0)->GetPosition();
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



