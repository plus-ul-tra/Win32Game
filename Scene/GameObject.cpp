#include "INC_Windows.h"

#include "Collider.h"
#include "RenderHelp.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include <iostream>


GameObject::~GameObject()
{
    if (m_pColliderCircle)
    {
        delete m_pColliderCircle;
        m_pColliderCircle = nullptr;
    }

    if (m_pColliderBox)
    {
        delete m_pColliderBox;
        m_pColliderBox = nullptr;
    }
}

void GameObject::Render(HDC hdc)
{
    DrawBitmap(hdc);
    //DrawCollider(hdc);
}


void GameObject::SetColliderCircle(float radius)
{
    if (m_pColliderCircle)
    {
        delete m_pColliderCircle;
        m_pColliderCircle = nullptr;
    }

    m_pColliderCircle = new ColliderCircle;

    assert(m_pColliderCircle != nullptr && "Failed to create ColliderCircle!");

    m_pColliderCircle->radius = radius;
    m_pColliderCircle->center = m_pos;
}


void GameObject::SetColliderBox(float width, float height)
{
    if (m_pColliderBox)
    {
        delete m_pColliderBox;
        m_pColliderBox = nullptr;
    }

    m_pColliderBox = new ColliderBox;

    assert(m_pColliderBox != nullptr && "Failed to create ColliderBox!");

    m_pColliderBox->center = m_pos;
    m_pColliderBox->halfSize.x = width / 2.0f;
    m_pColliderBox->halfSize.y = height / 2.0f;
}

void GameObject::DrawCollider(HDC hdc)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    if (m_pColliderCircle)
    {
        Ellipse(hdc, m_pColliderCircle->center.x - m_pColliderCircle->radius,
            m_pColliderCircle->center.y - m_pColliderCircle->radius,
            m_pColliderCircle->center.x + m_pColliderCircle->radius,
            m_pColliderCircle->center.y + m_pColliderCircle->radius);
    }

    if (m_pColliderBox)
    {
        Rectangle(hdc, m_pColliderBox->center.x - m_pColliderBox->halfSize.x,
            m_pColliderBox->center.y - m_pColliderBox->halfSize.y,
            m_pColliderBox->center.x + m_pColliderBox->halfSize.x,
            m_pColliderBox->center.y + m_pColliderBox->halfSize.y);
    }

    // 이전 객체 복원 및 펜 삭제
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}

//Bitmapcut
void GameObject::SetBitmapInfo(BitmapInfo* bitmapInfo,int widthCut, int heightCut)
{
    assert(m_pBitmapInfo == nullptr && "BitmapInfo must be null!");
    
    m_pBitmapInfo = bitmapInfo;
 
    m_frameWidth = m_pBitmapInfo->GetWidth() /widthCut;
    m_frameHeight = m_pBitmapInfo->GetHeight() / heightCut;
    m_frameIndex = 0;

    const int maxFrameCount = widthCut * heightCut;

    for (int y = 0; y < heightCut; ++y)
    {
        for (int x = 0; x < widthCut; ++x)
        {
            int index = y * widthCut + x;
            if (index >= maxFrameCount)
                return;

            m_frameXY[index].x = x * m_frameWidth;
            m_frameXY[index].y = y * m_frameHeight;
        }
    }

}

void GameObject::DrawBitmap(HDC hdc)
{
    if (m_pBitmapInfo == nullptr) return;
    if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;

    HDC hBitmapDC = CreateCompatibleDC(hdc);
    
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());
    // BLENDFUNCTION 설정 (알파 채널 처리)
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;  // 원본 알파 채널 그대로 사용
    blend.AlphaFormat = AC_SRC_ALPHA;

    const int x = m_pos.x - m_width / 2;
    const int y = m_pos.y - m_height / 2;

    const int srcX = m_frameXY[m_frameIndex].x;
    const int srcY = m_frameXY[m_frameIndex].y;

    AlphaBlend(hdc, x, y, m_width, m_height,
        hBitmapDC, srcX, srcY, m_frameWidth, m_frameHeight, blend); //실제 그림

    // 비트맵 핸들 복원
    SelectObject(hBitmapDC, hOldBitmap);
    DeleteDC(hBitmapDC);
   
}

//void GameObject::Move(float deltaTime)
//{
//    //GameObjectBase::Move(deltaTime);
//} 

void GameObject::UpdateFrame(float deltaTime)
{
    m_frameTime += deltaTime;
    if (m_frameTime >= m_frameDuration)
    {
        m_frameTime = 0.0f;
        m_frameIndex = (m_frameIndex + 1) % (m_frameCount);
    }
}

void GameObjectBase::SetName(const char* name)
{
   if (name == nullptr) return;
        
    strncpy_s(m_name, name, OBJECT_NAME_LEN_MAX - 1);
    m_name[OBJECT_NAME_LEN_MAX - 1] = '\0';
}


Background::~Background()
{
}

void Background::Update(float deltaTime)
{
}

void Background::Render(HDC hdc)
{
   DrawBitmap(hdc);
}

void Background::Move(float deltaTime)
{

}

void Background::SetBitmapInfo(BitmapInfo* bitmapInfo)
{
    assert(m_pBitmapInfo == nullptr && "BitmapInfo must be null!");
    
    m_pBitmapInfo = bitmapInfo;
}

void Background::DrawBitmap(HDC hdc)
{
    if (m_pBitmapInfo == nullptr) return;
    if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;
    HDC hBitmapDC = CreateCompatibleDC(hdc);
    
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());
    // BLENDFUNCTION 설정 (알파 채널 처리)
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;  // 원본 알파 채널 그대로 사용
    blend.AlphaFormat = AC_SRC_ALPHA;
    
    int screenWidth = 0;
    int screenHeight = 0;
    learning::GetScreenSize(screenWidth, screenHeight);
     
    AlphaBlend(hdc, 0, 0, screenWidth, screenHeight,
                hBitmapDC, 0, 0, m_width, m_height, blend);
    // 비트맵 핸들 복원
    SelectObject(hBitmapDC, hOldBitmap);
    DeleteDC(hBitmapDC);
}

void Player::Update(float deltaTime)
{
    UpdateFrame(deltaTime);
    Move(deltaTime);
    if (m_pColliderCircle)
    {
        m_pColliderCircle->center = m_pos;
    }
    if (m_pColliderBox)
    {
        m_pColliderBox->center = m_pos;
    }
}

void Player::Move(float deltaTime) // 스파이크, 슬라이딩ㅓㅜ
{
   const int padding = 40;

    if (!m_isSlide && m_input.moveLeft)
        m_pos.x -= m_speedX * deltaTime;
    if (!m_isSlide && m_input.moveRight)
        m_pos.x += m_speedX * deltaTime;
    if (m_isGrounded && m_input.skill && m_canSlide && !m_isSlide) {
    m_isSlide = true;
    m_canSlide = false;
    m_slideTimer = 0.0f;
    m_slideDir = m_input.moveLeft ? -1 : (m_input.moveRight ? 1 : 0);
    }

    
    if (m_isSlide) {
        m_pos.x += m_slideDir * m_slideSpeed * deltaTime;
        m_slideTimer += deltaTime;

    // 슬라이딩 종료 조건
    if (m_slideTimer >= m_slideDuration) {
        m_isSlide = false;
        m_slideCooldownTimer = 0.0f;
    }
}

    // 슬라이딩 쿨타임
    if (!m_canSlide) {
        m_slideCooldownTimer += deltaTime;
        if (m_slideCooldownTimer >= m_slideCooldownDuration) {
            m_canSlide = true;
        }
    }
    // 점프
    if (!m_isSlide && m_input.jump && m_isGrounded) {
        m_velocityY = -m_jumpPower;
        m_isGrounded = false;
    }

    // 중력
    m_velocityY += m_gravity * deltaTime;
    m_pos.y += m_velocityY * deltaTime;

    // 바닥 충돌
    float groundY = m_boundaryHeight - m_height;
    if (m_pos.y >= groundY) {
        m_pos.y = groundY;
        m_velocityY = 0.0f;
        m_isGrounded = true;
    }

    // 좌우 화면 경계
    if (m_pos.x < m_boundStart + padding) m_pos.x = m_boundStart + padding;
    if (m_pos.x > m_boundaryWidth - padding) m_pos.x = m_boundaryWidth - padding;
    //std::cout << m_pos.y << " " << groundY <<" " <<m_isGrounded<< std::endl;
    
}
void Ball::Update(float deltaTime)
{
    UpdateFrame(deltaTime);
    Move(deltaTime);
    if (m_pColliderCircle)
    {
        m_pColliderCircle->center = m_pos;
    }
    if (m_pColliderBox)
    {
        m_pColliderBox->center = m_pos;
    }
}

void Ball::Move(float deltaTime)
{   
    int padding = 50;
    m_pos.x += m_dir.x * m_speedX *deltaTime;
    m_pos.y += m_dir.y * m_speedY *deltaTime;
    //m_pos += m_dir * 0.5f * deltaTime;
    // spike를 했을때 is_Hit = true
    if (!m_isHit) {
        
    }
    //if (m_isHit) {
    //    //때렸을때 중력 적용X 직선운동
    //}

    // 벽에 닿았을때 방향만 바뀌도록
    if (m_pos.y + m_height > m_boundaryHeight) {
        m_pos.y = m_boundaryHeight - m_height;
        m_speedY *= -0.2f; //바닥
        std::cout << m_pos.x << std::endl; // -> 500이하 p1 lose, 이상 p2 Win으로 로직
    }
    if (m_pos.y - m_height < -padding) {
        m_pos.y = -padding + m_height;
        m_speedY *= -1.0f; //천장
    }
    if (m_pos.x - m_width < -padding) {
        m_pos.x = -padding + m_width;
        m_speedX *= -1.0f;
    }
    if (m_pos.x + m_width > m_boundaryWidth + padding) {
        m_pos.x = m_boundaryWidth - m_width + padding;
        m_speedX *= -1.0;
    }
    m_isCollision = false;
}

void Ball::CheckCollision(ColliderCircle const& p1, ColliderCircle const& p2)
{
    if (learning::Intersect(*m_pColliderCircle, p1) /*&& !m_isCollision*/) {
        m_isHit = false;
        /*std::cout << "p1와 충돌!" << std::endl;
        std::cout << m_dir.x << " " << m_dir.y << std::endl;*/
        m_isCollision = true;
        //m_dir = learning::CollisionOccured(*m_pColliderCircle, p1);
        m_dir.x = Colli(*m_pColliderCircle, p1);
        m_speedY = -0.8f;
        m_speedX = 0.8f;
        
    }
    if (learning::Intersect(*m_pColliderCircle, p2) /*&& !m_isCollision*/) {
        m_isHit = false;
        //std::cout << "p2와 충돌!!" << std::endl;
        m_isCollision = true;
        std::cout << m_dir.x << " " << m_dir.y << std::endl;
        m_dir.x = Colli(*m_pColliderCircle, p2);
        m_speedY = -0.8f;
        m_speedX = 0.8f;
    }
}



