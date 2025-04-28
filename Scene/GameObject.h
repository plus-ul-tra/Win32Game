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

struct Input {
    bool moveLeft;
    bool moveRight;
    bool jump;
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

    void SetPosition(float x, float y) { m_pos = { x, y }; }
    void SetDirection(Vector2f dir) { m_dir = dir; }
    void SetSpeed(float speed) { m_speed = speed; }
    void SetName(const char* name);

    void SetWidth(int width) { m_width = width; }
    void SetHeight(int height) { m_height = height; }

    int GetWidth() const { return m_width; } // 2025-04-22
    int GetHeight() const { return m_height; } 
    
    ObjectType Type() const { return m_type; }

    const char* GetName() const { return m_name; }

    Vector2f GetPosition() const { return m_pos; }
    Vector2f GetDirection() const { return m_dir; }

    float GetSpeed() const { return m_speed; }

  

protected:

    void Move(float deltaTime)
    {   //Move Mechaism 수정 필요
        // 공과 같은 물리 적용시, 공과 같은 움직임 나와 버릴 수 있음 확인
        m_pos.x += m_dir.x * m_speed * deltaTime;
        m_pos.y += m_dir.y * m_speed * deltaTime; 
    }

protected:
    ObjectType m_type;
    Input input;

    int m_width = 0;
    int m_height = 0;

    Vector2f m_pos = { 0.0f, 0.0f };
    Vector2f m_dir = { 0.0f, 0.0f }; // 방향 (단위 벡터)

    float m_speed = 0.0f; // 속력
    float m_gravity = 0.5f; //중력

    char m_name[OBJECT_NAME_LEN_MAX] = "";
};

class GameObject : public GameObjectBase
{
    using ColliderCircle = learning::ColliderCircle;
    using ColliderBox = learning::ColliderBox;
    using BitmapInfo = renderHelp::BitmapInfo;

public:
    GameObject();
    GameObject(const GameObject&) = delete;
    GameObject(ObjectType type) : GameObjectBase(type) {}
    ~GameObject() override;

    void Update(float deltaTime) override;
    void Render(HDC hdc) override;

    void SetColliderCircle(float radius);
    void SetColliderBox(float halfWidth, float halfHeight);

    void SetBitmapInfo(BitmapInfo* bitmapInfo);
   
protected:
    void DrawCollider(HDC hdc);
    void DrawBitmap(HDC hdc);

    void Move(float deltaTime); //move
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
    FrameFPos m_frameXY[14] = {{ 0, 0 }, };
    int m_frameWidth = 0;
    int m_frameHeight = 0;
    int m_frameIndex = 0;
    int m_frameCount = 14; // 프레임 수

    float m_frameTime = 0.0f;
    float m_frameDuration = 100.0f; // 임의 설정
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

    void SetBitmapInfo(BitmapInfo* bitmapInfo);

protected:
    void DrawBitmap(HDC hdc);
    // Bitmap 정보
    BitmapInfo* m_pBitmapInfo = nullptr;
};