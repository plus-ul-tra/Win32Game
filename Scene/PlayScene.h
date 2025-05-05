#pragma once
#include "Scene.h"
class GameObjectBase;
class GameObject;
class MyFirstWndGame;

constexpr int MAX_SCORE = 10;
class PlayScene :public Scene
{
 public:
    PlayScene() = default;
    ~PlayScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override;
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;
    void SetNewRound();
private:
    float m_roundTimer = 1500.0f; // next round 
    bool m_isScore = false;
    int m_winner = 0;
    int m_player1Score = 0;
    int m_player2Score = 0;

    void CreatePlayer(int num);
    void CreateBall();
    void CreateNet();
    void UpdatePlayerInfo();
    //void UpdateBallInfo();
    

    GameObjectBase* m_pBackground = nullptr;
    Player* GetPlayer(int num) const { return(Player*)m_GameObjectPtrTable[num]; }  //downcast;
    Ball* GetBall() const { return(Ball*)m_GameObjectPtrTable[2]; }
    Net* GetNet() const { return(Net*)m_GameObjectPtrTable[3]; }
    ScoreBoard* GetScore(int num) const {return(ScoreBoard*)m_GameObjectPtrTable[num];}
    MyFirstWndGame* m_pGame = nullptr;
    RECT m_rect = { 0, 0, 0, 0 };
};

