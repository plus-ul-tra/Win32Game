#pragma once
#include "NzWndBase.h"
#include "Utillity.h"
#include "Scene.h"

// [CHECK] #7  전방 선언을 사용하여 헤더파일의 의존성을 줄임.
class GameTimer;
//class GameObjectBase;
//class GameObject;


class Scene;
namespace renderHelp
{
    class BitmapInfo;
}

class MyFirstWndGame : public NzWndBase
{
public:
    MyFirstWndGame() = default;
    ~MyFirstWndGame() override = default;

    bool Initialize();
    void Run();
    void Finalize();

    void ChangeScene(SceneType eSceneType);

private:
    void Update();
    void Render();

    void OnResize(int width, int height) override;
    void OnClose() override;

    void OnMouseMove(int x, int y);
    void OnLButtonDown(int x, int y);
    void OnRButtonDown(int x, int y);
    
    void FixedUpdate();
    void LogicUpdate();
 
private:
    HDC m_hFrontDC = nullptr;
    HDC m_hBackDC = nullptr;
    HBITMAP m_hBackBitmap = nullptr;
    HBITMAP m_hDefaultBitmap = nullptr;
   
    // [CHECK] #8. 게임 타이머를 사용하여 프레임을 관리하는 예시.
    GameTimer* m_pGameTimer = nullptr;
    float m_fDeltaTime = 0.0f;
    float m_fFrameCount = 0.0f;

    // [CHECK] #8. 게임 오브젝트를 관리하는 컨테이너.
    //[20240422] Scene 에서 관리하도록 변경
    //int m_eCurrentScene = SCENE_PLAY; Enter 가 안되서.
    int m_eCurrentScene = SCENE_TITLE;
    Scene* m_pScenes[SceneType::SCENE_MAX] = { nullptr, nullptr, nullptr };
    //GameObjectBase** m_GameObjectPtrTable  = nullptr;

    struct MOUSE_POS
    {
        int x = 0;
        int y = 0;

        bool operator!=(const MOUSE_POS& other) const
        {
            return (x != other.x || y != other.y);
        }
    };
    
    MOUSE_POS m_MousePos = { 0, 0 };
    MOUSE_POS m_MousePosPrev = { 0, 0 };

    MOUSE_POS m_EnemySpawnPos = { 0, 0 };
    
#pragma region resource
    using BitmapInfo = renderHelp::BitmapInfo;

    BitmapInfo* m_pPlayer1BitmapInfo = nullptr;
    BitmapInfo* m_pPlayer2BitmapInfo = nullptr;
    BitmapInfo* m_pKeyGuideBitmapInfo = nullptr;
    BitmapInfo* m_pBallBitmapInfo = nullptr;
    BitmapInfo* m_pButtonBitmapInfo = nullptr;
    BitmapInfo* m_pBackgroundBitmapInfo = nullptr;
    BitmapInfo* m_pScoreBitmapInfo = nullptr;
    BitmapInfo* m_pPlayBackgroundBitmapInfo = nullptr;
    BitmapInfo* m_pWinUi = nullptr;
    BitmapInfo* m_pRestart = nullptr;
    
public:

    using Vector2f = learning::Vector2f;
    int m_winner = 0;
    Vector2f EnemySpawnPosition() const { return Vector2f(m_EnemySpawnPos.x, m_EnemySpawnPos.y); }
    void ResetEnemySpawnPosition() { m_EnemySpawnPos = { 0, 0 }; } //X
    BitmapInfo* GetPlayerBitmapInfo() const { return m_pPlayer1BitmapInfo; }
    BitmapInfo* GetPlayer2BitmapInfo() const { return m_pPlayer2BitmapInfo; }
    BitmapInfo* GetBallBitmapInfo() const { return m_pBallBitmapInfo; }
    BitmapInfo* GetBackgroundBitmapInfo() const { return m_pBackgroundBitmapInfo; }
    BitmapInfo* GetPlayBackgroundBitmapInfo() const { return m_pPlayBackgroundBitmapInfo; }
    BitmapInfo* GetKeyGuideBitmapInfo() const { return m_pKeyGuideBitmapInfo; }
    BitmapInfo* GetButtonBitmapInfo() const { return m_pButtonBitmapInfo; }
    BitmapInfo* GetScoreBitmapInfo() const { return m_pScoreBitmapInfo; }
    BitmapInfo* GetWinBitmapInfo() const { return m_pWinUi; }
    BitmapInfo* GetRestartBitmapInfo() const { return m_pRestart; }
#pragma endregion
};
