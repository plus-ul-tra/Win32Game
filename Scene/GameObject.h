#pragma once

#include "Utillity.h"
#include "INC_Windows.h"
#include <algorithm>
// [CHECK]. namespace �����ؼ� ���� �����ؾ� ��
namespace learning
{
    struct ColliderCircle;
    struct ColliderBox;
}

namespace renderHelp
{
    class BitmapInfo;
}

enum class ObjectType
{ //ObjType
    PLAYER,
    BAll,
    NET,
    BACKGROUND,
    UI,
};
struct PlayerInput {
    bool moveLeft = false;
    bool moveRight = false;
    bool jump = false;
    bool skill = false;
    
};
constexpr int OBJECT_NAME_LEN_MAX = 15;

class GameObjectBase
{
    using Vector2f = learning::Vector2f;
public:
    GameObjectBase() = delete;
    GameObjectBase(const GameObjectBase&) = delete;

    GameObjectBase(ObjectType type) : m_type(type) {}

    virtual ~GameObjectBase() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Render(HDC hdc) = 0;
    virtual void Move(float deltaTime) = 0;
    void SetPosition(float x, float y) { m_pos = { x, y }; }
    void SetDirection(Vector2f dir) { m_dir = dir; }
    void SetSpeed(float speedX, float speedY) { m_speedX = speedX; m_speedY = speedY; }
    void SetName(const char* name);

    void SetWidth(int width) { m_width = width; }
    void SetHeight(int height) { m_height = height; }
    void SetBoundaryInfo(int width, int height) { m_boundaryWidth = width; m_boundaryHeight = height; }
    int GetWidth() const { return m_width; } 
    int GetHeight() const { return m_height; } 
    
    ObjectType Type() const { return m_type; }

    const char* GetName() const { return m_name; }

    Vector2f GetPosition() const { return m_pos; }
    Vector2f GetDirection() const { return m_dir; }

    //float GetSpeed() const { return m_speed; }

     

protected:
    ObjectType m_type;
    int m_width = 0;
    int m_height = 0;

    Vector2f m_pos = { 0.0f, 0.0f };
    Vector2f m_dir = { 0.0f, 0.0f }; // ���� (���� ����)

    float m_speedX = 0.0f; // x�ӷ�
    float m_speedY = 0.0f;
    
    int m_boundaryWidth;
    int m_boundaryHeight; //play ��������
    char m_name[OBJECT_NAME_LEN_MAX] = "";
};

class GameObject : public GameObjectBase
{
    using ColliderCircle = learning::ColliderCircle;
    using ColliderBox = learning::ColliderBox;
    using BitmapInfo = renderHelp::BitmapInfo;

public:
    GameObject(const GameObject&) = delete;
    GameObject(ObjectType type) : GameObjectBase(type) {}
    ~GameObject() override;

    virtual void Update(float deltaTime) = 0;
    void Render(HDC hdc) override;

    void SetColliderCircle(float radius);
    void SetColliderBox(float halfWidth, float halfHeight);

    void SetBitmapInfo(BitmapInfo* bitmapInfo,int widthCut, int Height);
    virtual void Move(float deltaTime) = 0 ; //move
    ColliderCircle* m_pColliderCircle = nullptr;
    ColliderBox* m_pColliderBox = nullptr;
protected:
    void DrawCollider(HDC hdc);
    void DrawBitmap(HDC hdc);

    
    void UpdateFrame(float deltaTime);
 
    // Collider
    //ColliderCircle* m_pColliderCircle = nullptr;
    //ColliderBox* m_pColliderBox = nullptr;

    // Bitmap ����
    BitmapInfo* m_pBitmapInfo = nullptr;

    // ���������� ���ڰ� ���� ���ƿ�.
    struct FrameFPos
    {
        int x;
        int y;
    };
    FrameFPos m_frameXY[28] = {{ 0, 0 }, };
    int m_frameWidth = 0;
    int m_frameHeight = 0;
    int m_frameIndex = 0;
    int m_frameCount = 5; // ������ �� 

    float m_frameTime = 0.0f;
    float m_frameDuration = 100.0f; // ���� ����
};

class Player : public GameObject {
public:
    int playerIndex = 0;
    bool prevEnterPressed = false;
    bool prevZPressed = false;
    bool m_isSkill = false;
    bool m_isGrounded = false;
    bool m_isSlideOnCooldown = false;
    Player(const GameObject&) = delete;
    Player(ObjectType type) : GameObject(type) {}
    void Update(float deltaTime) override;
    void Move(float deltaTime) override;
    void Skill(float deltaTime);
    void SetInput(const PlayerInput& input) { m_input = input; }
    void SetPlayerBoundaryInfo(int start, int width, int height){m_boundStart =start, m_boundaryWidth = width; m_boundaryHeight = height; }
    //void Update(float deltaTime) override;
protected:
    PlayerInput m_input;
    int m_boundStart = 0;
    
    float m_velocityY = 0.0f;     
    float m_gravity = 0.0012f; // �߷� ���ӵ�
    float m_jumpPower = 0.8f;// ���� ���� �ӵ� (����: ����)

    float m_slideDir = 0.0f;
    float m_slideSpeed = 0.6f;
    float m_slideCooldownTimer = 0.0f;
    float m_slideCooldownDuration = 1.0f;
    float m_slideDuration = 0.0f;

};

class Ball : public GameObject {
    using ColliderCircle = learning::ColliderCircle;
    using ColliderBox = learning::ColliderBox;
public:
    Ball(const GameObject&) = delete;
    Ball(ObjectType type) : GameObject(type) {}
    void Update(float deltaTime) override;
    void Move(float deltaTime) override;
    void CheckCollision(ColliderCircle const& p1, ColliderCircle const& p2);
    void CollisionNet(ColliderBox const& net);
    bool m_isHit = true;
    bool m_isScore = false;
    int  m_winPlayer = 0;
protected:

    //FrameFPos m_frameXY[1] = { { 0, 0 }, };
    void SetWinner();
    bool m_isCollision = false;
    int m_frameWidth = 0;
    int m_frameHeight = 0;
    int m_frameIndex = 0;
    int m_frameCount = 1; // ������ �� 
    float m_frameTime = 0.0f;
    float m_frameDuration = 100.0f;
    float m_spikeRate = 1.8f;

    
};

class Net : public GameObject {
    using ColliderBox = learning::ColliderBox;
public:
    Net(const GameObject&) = delete;
    Net(ObjectType type) : GameObject(type) {}
    void Update(float deltaTime) override;
    void Move(float deltaTime) override;
};


class Background : public GameObject
{
    using BitmapInfo = renderHelp::BitmapInfo;

public:
    Background(const Background&) = delete;
    Background(ObjectType type) : GameObject(type) {}
    ~Background() override;

    void Update(float deltaTime) override;
    void Render(HDC hdc) override;
    void Move(float deltaTime) override;
    void SetBitmapInfo(BitmapInfo* bitmapInfo);

protected:
    void DrawBitmap(HDC hdc);
    // Bitmap ����
    BitmapInfo* m_pBitmapInfo = nullptr;
};

//button UI class ���� 
class Ui : public GameObject
{
    using BitmapInfo = renderHelp::BitmapInfo;
public:
    Ui(const Ui&) = delete;
    Ui(ObjectType type) : GameObject(type){}
    ~Ui() override;

    void Update(float deltaTime) override;
    void Render(HDC hdc) override;
    void Move(float deltaTime) override;
    void SetBitmapInfo(BitmapInfo* bitmapInfo);
    void SetScale(float sx, float sy) {
        m_scaleX = sx;
        m_scaleY = sy;
           
    }
protected:
    float m_scaleX = 1.0f;
    float m_scaleY = 1.0f;
    void DrawBitmap(HDC hdc);
    BitmapInfo* m_pBitmapInfo = nullptr;
};

class ScoreBoard :public Ui {
    using BitmapInfo = renderHelp::BitmapInfo;
public:
    void UpdateFrame(int score);
    ScoreBoard(const Ui&) = delete;
    ScoreBoard(ObjectType type) : Ui(type) {}
    ~ScoreBoard()override;
    void Update(float deltaTime) override;
    void Render(HDC hdc) override;
    void Move(float deltaTime) override;
    void SetBitmapInfo(BitmapInfo* bitmapInfo, int widthCut, int heightCut);
protected:
    void DrawBitmap(HDC hdc);
    BitmapInfo* m_pBitmapInfo = nullptr;

};