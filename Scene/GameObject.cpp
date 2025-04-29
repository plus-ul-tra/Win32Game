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

void GameObject::Update(float deltaTime)
{
    UpdateFrame(deltaTime);
    Move(deltaTime);

    // Collider 업데이트
    if (m_pColliderCircle)
    {
        m_pColliderCircle->center = m_pos;
    } 
    if (m_pColliderBox)
    {
        m_pColliderBox->center = m_pos;
    }
}

void GameObject::Render(HDC hdc)
{
    DrawBitmap(hdc);
    DrawCollider(hdc);
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
        hBitmapDC, srcX, srcY, m_frameWidth - 2, m_frameHeight, blend);

    // 비트맵 핸들 복원
    SelectObject(hBitmapDC, hOldBitmap);
    DeleteDC(hBitmapDC);
   
}

void GameObject::Move(float deltaTime)
{
    GameObjectBase::Move(deltaTime);
}

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

void GameObjectBase::Move(float deltaTime)
{       //실제 움직임
    {
        switch (m_type) {
        case ObjectType::PLAYER:
            //int m_boundaryWidth, int m_boundaryHeight 사용해서 
            //Player와 Ball은 모두 boundary에서 빠져 나갈 수 없도록
            m_pos.x += m_dir.x * m_speed * deltaTime;
            m_pos.y += m_dir.y * m_speed * deltaTime;
            if (m_pos.x < 0)m_pos.x = 0;
            if (m_pos.x > m_boundaryWidth) m_pos.x = m_boundaryWidth;
            if (m_pos.y < 0) m_pos.y = 0;
            if (m_pos.y + m_height > m_boundaryHeight) m_pos.y = m_boundaryHeight - m_height;
            
            break;
        case ObjectType::BAll:
            
            break;
        }
    }
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
