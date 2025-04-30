#pragma once

#include "Utillity.h"
#include "INC_Windows.h"
#include <algorithm>
// [CHECK]. namespace 포함해서 전방 선언해야 함
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
{ //ObjType에서 정의되어야 그릴 수 있따.
    PLAYER,
    ENEMY, //데코
    BAll,
    NET,
    BACKGROUND,
};
struct PlayerInput {
    bool moveLeft = false;
    bool moveRight = false;
    bool jump = false;
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
    Vector2f m_dir = { 0.0f, 0.0f }; // 방향 (단위 벡터)

    float m_speedX = 0.0f; // x속력
    float m_speedY = 0.0f;
    
    int m_boundaryWidth;
    int m_boundaryHeight; //play 영역제한
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
protected:
    void DrawCollider(HDC hdc);
    void DrawBitmap(HDC hdc);

    
    void UpdateFrame(float deltaTime);
 
    // Collider
    ColliderCircle* m_pColliderCircle = nullptr;
    ColliderBox* m_pColliderBox = nullptr;

    // Bitmap 정보
    BitmapInfo* m_pBitmapInfo = nullptr;

    // 점진적으로 예쁘게 고쳐 보아요.
    struct FrameFPos
    {
        int x;
        int y;
    };
    FrameFPos m_frameXY[28] = {{ 0, 0 }, };
    int m_frameWidth = 0;
    int m_frameHeight = 0;
    int m_frameIndex = 0;
    int m_frameCount = 28; // 프레임 수 

    float m_frameTime = 0.0f;
    float m_frameDuration = 100.0f; // 임의 설정
};

class Player : public GameObject {
public:
    Player(const GameObject&) = delete;
    Player(ObjectType type) : GameObject(type) {}
    void Update(float deltaTime) override;
    void Move(float deltaTime) override;
    void SetInput(const PlayerInput& input) { m_input = input; }
    //void Update(float deltaTime) override;
protected:
    PlayerInput m_input;
    bool m_isGrounded = false;    
    float m_velocityY = 0.0f;      // 
    float m_gravity = 0.0012f; // 중력 가속도
    float m_jumpPower = 0.8f;// 점프 시작 속도 (음수: 위로)
    //float m_height = 100.0f;
    


    FrameFPos m_frameXY[2] = { { 0, 0 }, };
    int m_frameWidth = 0;
    int m_frameHeight = 0;
    int m_frameIndex = 0;
    int m_frameCount = 2; // 프레임 수 
    float m_frameTime = 0.0f;
    float m_frameDuration = 100.0f;

};

class Ball : public GameObject {
public:
    Ball(const GameObject&) = delete;
    Ball(ObjectType type) : GameObject(type) {}
    void Update(float deltaTime) override;
    void Move(float deltaTime) override;
    //void Update(float deltaTime) override;
protected:
    FrameFPos m_frameXY[1] = { { 0, 0 }, };
    int m_frameWidth = 0;
    int m_frameHeight = 0;
    int m_frameIndex = 0;
    int m_frameCount = 1; // 프레임 수 
    float m_frameTime = 0.0f;
    float m_frameDuration = 100.0f;
    
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
    // Bitmap 정보
    BitmapInfo* m_pBitmapInfo = nullptr;
};