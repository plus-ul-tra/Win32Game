#include "INC_Windows.h"

// --- 전역 변수 ---
float ball_x = 400;
float ball_y = 100;
float ball_radius = 20;

float ball_x_speed = 3.0f;
float ball_y_speed = 0.0f;

const float gravity = 0.5f;
const int ground_y = 550;
const int window_width = 800;
const int window_height = 600;

// --- 함수 선언 ---
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Update();
void Draw(HDC hdc);

//int main() {
//    HINSTANCE hInstance = GetModuleHandle(nullptr);
//
//    WNDCLASS wc = { 0 };
//    wc.lpfnWndProc = WndProc;
//    wc.hInstance = hInstance;
//    wc.lpszClassName = L"PikachuVolley";
//
//    RegisterClass(&wc);
//
//    HWND hwnd = CreateWindow(
//        L"PikachuVolley", L"피카츄 배구 - 공 테스트",
//        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height,
//        nullptr, nullptr, hInstance, nullptr
//    );
//
//    ShowWindow(hwnd, SW_SHOWDEFAULT);
//    UpdateWindow(hwnd);
//
//    SetTimer(hwnd, 1, 16, nullptr); // 60FPS용 타이머
//
//    MSG msg = { 0 };
//    while (GetMessage(&msg, nullptr, 0, 0)) {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//    return (int)msg.wParam;
//}

// --- 윈도우 프로시저 ---
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_TIMER:
        Update();
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Draw(hdc);
        EndPaint(hwnd, &ps);
    }
    return 0;
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// --- 업데이트 함수 ---
void Update() {
    ball_y_speed += gravity;

    ball_x += ball_x_speed;
    ball_y += ball_y_speed;

    // 바닥 충돌
    if (ball_y + ball_radius > ground_y) {
        ball_y = ground_y - ball_radius;
        ball_y_speed *= -0.8f; // 튕길 때 에너지 감소
    }

    // 좌우 벽 충돌
    if (ball_x - ball_radius < 0) {
        ball_x = ball_radius;
        ball_x_speed *= -1;
    }
    if (ball_x + ball_radius > window_width) {
        ball_x = window_width - ball_radius;
        ball_x_speed *= -1;
    }
}

// --- 그리기 함수 ---
void Draw(HDC hdc) {
    // 배경 흰색
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    RECT rect = { 0, 0, window_width, window_height };
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    // 바닥
    hBrush = CreateSolidBrush(RGB(200, 200, 200));
    RECT ground = { 0, ground_y, window_width, window_height };
    FillRect(hdc, &ground, hBrush);
    DeleteObject(hBrush);

    // 공
    hBrush = CreateSolidBrush(RGB(255, 200, 0)); // 노란 공
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    Ellipse(hdc,
        (int)(ball_x - ball_radius),
        (int)(ball_y - ball_radius),
        (int)(ball_x + ball_radius),
        (int)(ball_y + ball_radius));
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}
