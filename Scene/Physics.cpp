#include <windows.h>
#include <tchar.h>

// --- 전역 변수 ---
float ball_x = 400;
float ball_y = 100;
float ball_radius = 40;

float ball_x_speed = 3.0f;
float ball_y_speed = 0.0f;

const float gravity = 0.5f;
const int ground_y = 550;
const int window_width = 800;
const int window_height = 600;

// 플레이어 입력 구조체
struct Input {
    bool moveLeft;
    bool moveRight;
    bool jump;
};

// 플레이어 구조체
struct Player {
    float x, y; //pos
    float width, height; //r 
    float y_speed;
    Input input;
};

Player player1 = { 200, ground_y - 100 , 60, 80, 0.0f };
Player player2 = { 600, ground_y - 60, 50, 60, 0.0f };

// --- 함수 선언 ---
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Update();
void Draw(HDC hdc);
void ReadInput();
void UpdatePlayers();

// --- main 함수 ---
//int main() {
//    HINSTANCE hInstance = GetModuleHandle(nullptr);
//
//    WNDCLASS wc = { 0 };
//    wc.lpfnWndProc = WndProc;
//    wc.hInstance = hInstance;
//    wc.lpszClassName = _T("PikachuVolley");
//
//    RegisterClass(&wc);
//
//    HWND hwnd = CreateWindow(
//        _T("PikachuVolley"), _T("피카츄 배구 - 2인용 입력 테스트"),
//        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height,
//        nullptr, nullptr, hInstance, nullptr
//    );
//
//    ShowWindow(hwnd, SW_SHOWDEFAULT);
//    UpdateWindow(hwnd);
//
//    SetTimer(hwnd, 1, 16, nullptr);
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
        ReadInput();
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

// --- 입력 읽기 ---
void ReadInput() {
    player1.input.moveLeft = (GetAsyncKeyState('A') & 0x8000);
    player1.input.moveRight = (GetAsyncKeyState('D') & 0x8000);
    player1.input.jump = (GetAsyncKeyState('W') & 0x8000);

    player2.input.moveLeft = (GetAsyncKeyState(VK_LEFT) & 0x8000);
    player2.input.moveRight = (GetAsyncKeyState(VK_RIGHT) & 0x8000);
    player2.input.jump = (GetAsyncKeyState(VK_UP) & 0x8000);
}

// --- 플레이어 업데이트 ---
void UpdatePlayer(Player& p) {
    if (p.input.moveLeft) p.x -= 5;
    if (p.input.moveRight) p.x += 5;
    if (p.input.jump && p.y + p.height >= ground_y) p.y_speed = -13.0f; // 바닥에 있을 때만 점프

    // 중력 적용
    p.y_speed += gravity;
    p.y += p.y_speed;

    // 바닥 충돌
    if (p.y + p.height > ground_y) {
        p.y = ground_y - p.height;
        p.y_speed = 0;
    }

    // 화면 경계 충돌
    if (p.x < 0) p.x = 0;
    if (p.x + p.width > window_width) p.x = window_width - p.width;
}

void UpdatePlayers() {
    UpdatePlayer(player1);
    UpdatePlayer(player2);
}

// --- 업데이트 함수 ---
void Update() {
    UpdatePlayers();

    ball_y_speed += gravity;

    ball_x += ball_x_speed;
    ball_y += ball_y_speed;

    if (ball_y + ball_radius > ground_y) {
        ball_y = ground_y - ball_radius;
        ball_y_speed *= -0.8f;
    }

    if (ball_x - ball_radius < 0) {
        ball_x = ball_radius;
        ball_x_speed *= -0.5;
    }
    if (ball_x + ball_radius > window_width) {
        ball_x = window_width - ball_radius;
        ball_x_speed *= -1;
    }
}

// --- 그리기 함수 ---
void Draw(HDC hdc) {
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    RECT rect = { 0, 0, window_width, window_height };
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    hBrush = CreateSolidBrush(RGB(200, 200, 200));
    RECT ground = { 0, ground_y, window_width, window_height };
    FillRect(hdc, &ground, hBrush);
    DeleteObject(hBrush);

    // 공 그리기
    hBrush = CreateSolidBrush(RGB(255, 200, 0));
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    Ellipse(hdc,
        (int)(ball_x - ball_radius),
        (int)(ball_y - ball_radius),
        (int)(ball_x + ball_radius),
        (int)(ball_y + ball_radius));
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);

    // 플레이어1 (빨간색)
    hBrush = CreateSolidBrush(RGB(255, 0, 0));
    oldBrush = SelectObject(hdc, hBrush);
    Ellipse(hdc,
        (int)player1.x,
        (int)player1.y,
        (int)(player1.x + player1.width),
        (int)(player1.y + player1.height));
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);

    // 플레이어2 (파란색)
    hBrush = CreateSolidBrush(RGB(0, 0, 255));
    oldBrush = SelectObject(hdc, hBrush);
    Ellipse(hdc,
        (int)player2.x,
        (int)player2.y,
        (int)(player2.x + player2.width),
        (int)(player2.y + player2.height));
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}
