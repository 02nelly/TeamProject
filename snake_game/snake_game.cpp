// snake_game.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "snake_game.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"
#include "windowsx.h"


#pragma comment (lib, "Msimg32.lib")

#define MAX_LOADSTRING 100
#define WINSIZEX 1280
#define WINSIZEY 1000
#define MAPSIZEX 17
#define MAPSIZEY 17
#define PlayerSize (MAPSIZEX-2)*(MAPSIZEY-2)

void Init();
void Update();
void Render();
void Release();

// 전역 변수:
HWND g_hWnd;
HDC g_hDC;
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

DWORD old_time;
DWORD cur_time;
float deltatime;

HDC			back_hDC;
HBITMAP		back_hBit;
HBITMAP		back_oBit;

HFONT font;
HFONT oldfont;

int Play_Map[MAPSIZEX][MAPSIZEY] = { 0, };

int Player_Tail[PlayerSize] = { 0, };

int X_Random;
int Y_Random;

int score = 0;

struct IMAGE {
    HDC dc;
    HBITMAP bit;
};

struct PLAYER {
    IMAGE image;
    int x, y;
    float speed;
    float Rx, Ry;
    int SizeUp;
    float dx, dy;
};


int dx = 0;
int dy = -1;

int _dx = 0;
int _dy = -1;

int xPos;
int yPos;

PLAYER player;
PLAYER Tail[PlayerSize];

struct Score_Object {
    IMAGE image;
    int x, y;
    int Rx, Ry;
};

Score_Object S_O;

struct RANK {
    char nick[128] = "AAA";
    int score;
    int sub_score;
    char finish_name[128] = "AAA";
    char name[128];
};

RANK rank;

int scene;

int move;

//char finish_name[5];

IMAGE Background;
IMAGE BG_ground;
IMAGE BG_ground2;
IMAGE MAIN;
IMAGE START_BUTTON;
IMAGE GAMEOVER;
IMAGE Score_banner;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SNAKEGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKEGAME));

    MSG msg;

    Init();

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            Update();
            Render();
        }
    }

    Release();
    
    return (int)msg.wParam;
}

void Init() {

    // 맵 첫장면 초기화
    scene = 1;
    
    player.SizeUp = 3;

    player.speed = 1;

    rank.score = 0;
    srand((unsigned int)time(NULL));
    S_O.x = (rand() % 15 + 1);
    S_O.y = (rand() % 15 + 1);

    S_O.Rx = S_O.x * 50;
    S_O.Ry = S_O.y * 50;

    g_hDC = GetDC(g_hWnd);
    back_hBit = CreateCompatibleBitmap(g_hDC, WINSIZEX, WINSIZEY);
    back_hDC = CreateCompatibleDC(g_hDC);
    back_oBit = (HBITMAP)SelectObject(back_hDC, back_hBit);

    //MAP 디자인 & Image 세팅
    for (int i = 0; i < MAPSIZEX; i++) {
        for (int j = 0; j < MAPSIZEY; j++) {
            if (i == 0 or i == 16 or j == 0 or j == 16)
                Play_Map[i][j] = 1;
        }
    }

    Background.dc = CreateCompatibleDC(g_hDC);
    Background.bit = (HBITMAP)LoadImage(nullptr, "BG_wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(Background.dc, Background.bit);

    BG_ground.dc = CreateCompatibleDC(g_hDC);
    BG_ground.bit = (HBITMAP)LoadImage(nullptr, "BG_ground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(BG_ground.dc, BG_ground.bit);

    BG_ground2.dc = CreateCompatibleDC(g_hDC);
    BG_ground2.bit = (HBITMAP)LoadImage(nullptr, "BG_ground2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(BG_ground2.dc, BG_ground2.bit);

    // 캐릭터 이미지
    player.image.dc = CreateCompatibleDC(g_hDC);
    player.image.bit = (HBITMAP)LoadImage(nullptr, "Player.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(player.image.dc, player.image.bit);

    player.x = 7;
    player.y = 12;
 
    for (int i = 0; i < 3; i++) {
        Tail[i].x = player.x;
        Tail[i].y = player.y + i;

        Tail[i].dx = 0;
        Tail[i].dy = -1;
    }

    Tail[0].Rx = player.x * 50;
    Tail[0].Ry = player.y * 50;

    // 점수 오브젝트 이미지
    S_O.image.dc = CreateCompatibleDC(g_hDC);
    S_O.image.bit = (HBITMAP)LoadImage(nullptr, "Score_Object.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(S_O.image.dc, S_O.image.bit);

    // 메인화면 이미지
    MAIN.dc = CreateCompatibleDC(g_hDC);
    MAIN.bit = (HBITMAP)LoadImage(nullptr, "MAIN.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(MAIN.dc, MAIN.bit);

    // 메인화면 게임 시작버튼 이미지
    START_BUTTON.dc = CreateCompatibleDC(g_hDC);
    START_BUTTON.bit = (HBITMAP)LoadImage(nullptr, "START_BUTTON.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(START_BUTTON.dc, START_BUTTON.bit);

    // 게임오버화면 이미지
    GAMEOVER.dc = CreateCompatibleDC(g_hDC);
    GAMEOVER.bit = (HBITMAP)LoadImage(nullptr, "GAMEOVER.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(GAMEOVER.dc, GAMEOVER.bit);

    // 점수 베너 이미지
    Score_banner.dc = CreateCompatibleDC(g_hDC);
    Score_banner.bit = (HBITMAP)LoadImage(nullptr, "Score_banner.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(Score_banner.dc, Score_banner.bit);



    old_time = GetTickCount ();

}

float one = 0.0f;

void Update() {
    
    switch (scene) {

    case 1:

        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            if (xPos >= 240 && xPos <= 1040 && yPos >= 550 && yPos <= 792)
                scene = 2;
        }
        else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            exit(1);

        }

        break;

    case 2:

        cur_time = GetTickCount();
        deltatime = (cur_time - old_time) / 1000.0;
        old_time = cur_time;

        if (_dx == 0) {

            if (GetAsyncKeyState('A') & 0x8000) {
                _dx = -1;
                _dy = 0;
            }
            else if (GetAsyncKeyState('D') & 0x8000) {
                _dx = 1;
                _dy = 0;
            }

        }
        else {

            if (GetAsyncKeyState('W') & 0x8000) {
                _dx = 0;
                _dy = -1;
            }
            else if (GetAsyncKeyState('S') & 0x8000) {
                _dx = 0;
                _dy = 1;
            }

        }
        if (GetAsyncKeyState('R') & 0x8000) {
            Init();
            scene = 2;
        }
        else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            Init();
            Sleep(200);

        }

        one += deltatime * player.speed;
        if (one >= 1.0f) {

            one = 0;

            for (int a = player.SizeUp - 1; a > 0; a--) {
                Tail[a].x = Tail[a - 1].x;
                Tail[a].y = Tail[a - 1].y;

                Tail[a].dx = Tail[a - 1].dx;
                Tail[a].dy = Tail[a - 1].dy;

            }

            Tail[0].x += Tail[0].dx;
            Tail[0].y += Tail[0].dy;
            Tail[0].dx = _dx;
            Tail[0].dy = _dy;

        }

        for (int i = 0; i < player.SizeUp; i++) {

            Tail[i].Rx = (Tail[i].x + one * Tail[i].dx) * 50;
            Tail[i].Ry = (Tail[i].y + one * Tail[i].dy) * 50;
        }

        dx = Tail[0].dx;
        dy = Tail[0].dy;

        // 벽 충돌
        if (Tail[0].x + dx < 1 || Tail[0].x + dx > 15 || Tail[0].y + dy < 1 || Tail[0].y + dy > 15) {

            //exit(1);
            scene = 3;
            
        }

        // 캐릭터 충돌 판정
        for (int i = 1; i < player.SizeUp; i++) {
            if (Tail[i].x == Tail[0].x && Tail[i].y == Tail[0].y) {
                scene = 3;
            }
        }

        // 플레이어와 오브젝트의 충돌 판정
        if ((Tail[0].x < S_O.x + 1 && Tail[0].y < S_O.y + 1) && (Tail[0].x + 1 > S_O.x && Tail[0].y + 1 > S_O.y)) {

            //오프젝트 랜덤생성
            S_O.x = (rand() % 15 + 1);
            S_O.y = (rand() % 15 + 1);

            for (int i = 1; i < player.SizeUp; i++) {

                S_O.x = (rand() % 15 + 1);
                S_O.y = (rand() % 15 + 1);
                S_O.Rx = S_O.x * 50;
                S_O.Ry = S_O.y * 50;

            }

            Tail[player.SizeUp].x = Tail[player.SizeUp - 1].x;
            Tail[player.SizeUp].y = Tail[player.SizeUp - 1].y;
            Tail[player.SizeUp].dx = Tail[player.SizeUp - 1].dx;
            Tail[player.SizeUp].dy = Tail[player.SizeUp - 1].dy;
            //점수증가
            rank.score++;

            //캐릭터 길이 증가
            player.SizeUp++;

            //캐릭터 속도 증가
            player.speed = 1 + (rank.score * 0.5);
        }

        break;

        case 3:

            if (GetAsyncKeyState('R') & 0x8000) {

                Init();
                scene = 2;
                Sleep(200);
            }
            else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {

                exit(1);
            }

            else if (GetAsyncKeyState('A') & 0x8000) {

                move--;
                Sleep(300);
                if (move < 0) move = 2;
            }
            else if (GetAsyncKeyState('D') & 0x8000) {

                move++;
                Sleep(300);
                if (move > 2) move = 0;
            }
            else if (GetAsyncKeyState('W') & 0x8000) {

                rank.nick[move] ++;
                Sleep(200);
                if (rank.nick[move] > 'Z') rank.nick[move] = 'A';

            }
            else if (GetAsyncKeyState('S') & 0x8000) {

                rank.nick[move] --;
                Sleep(200);
                if (rank.nick[move] < 'A') rank.nick[move] = 'Z';
       
            }
            else if (GetAsyncKeyState(VK_RETURN) & 0x8000) {

                for (int i = 0; i < 3; i++) {
                    rank.finish_name[i] = rank.nick[i];
                }

                strcpy_s(rank.name, rank.finish_name);

                rank.sub_score = rank.score;
                
            }

            break;
        
    }
  
}
    

void Render() {

    switch (scene) {

     // 메뉴 화면
    case 1:

        // 백디씨 흰색으로 덧칠해주기
        Rectangle(back_hDC, -1, -1, WINSIZEX, WINSIZEY);

        BitBlt(back_hDC, 0, 0, WINSIZEX, WINSIZEY, MAIN.dc, 0, 0, SRCCOPY);
        BitBlt(back_hDC, 240, 550, 1040, 792, START_BUTTON.dc, 0, 0, SRCCOPY);

        BitBlt(g_hDC, 0, 0, WINSIZEX, WINSIZEY, back_hDC, 0, 0, SRCCOPY);
        break;

    // 플레이 화면
    case 2:

        // 백디씨 흰색으로 덧칠해주기
        Rectangle(back_hDC, -1, -1, WINSIZEX, WINSIZEY);

        // 배경 칠해주기
        for (int i = 0; i < MAPSIZEX; i++) {
            for (int j = 0; j < MAPSIZEY; j++) {
                if (Play_Map[i][j] == 1) {
                    BitBlt(back_hDC, i * 50, j * 50, WINSIZEX, WINSIZEY, Background.dc, 0, 0, SRCCOPY);
                }
            }
        }

        for (int i = 1; i < MAPSIZEX - 1; i++) {
            for (int j = 1; j < MAPSIZEY - 1; j++) {
                if (i % 2 == 0 && j % 2 == 0 || i % 2 == 1 && j % 2 == 1) {
                    BitBlt(back_hDC, i * 50, j * 50, WINSIZEX, WINSIZEY, BG_ground.dc, 0, 0, SRCCOPY);
                }
                else BitBlt(back_hDC, i * 50, j * 50, WINSIZEX, WINSIZEY, BG_ground2.dc, 0, 0, SRCCOPY);

            }
        }


        // 스코어 오브젝트
        TransparentBlt(back_hDC, S_O.Rx, S_O.Ry, 50, 50, S_O.image.dc, 0, 0, 50, 50, RGB(255, 0, 255));

        // 캐릭터
        /*TransparentBlt(back_hDC, player.Rx, player.Ry, 50, 50, player.image.dc, 0, 0, 50, 50, RGB(255, 0, 255));


        //Player_Tail[player.x][player.y] = { 4 };

        // 꼬리
        for (int i = 0; i < player.SizeUp; i++) {

            TransparentBlt(back_hDC, player.Rx + (dx * -1) * 50 * i, player.Ry + (dy * -1) * 50 * i, 50, 50, player.image.dc, 0, 0, 50, 50, RGB(255, 0, 255));

        }*/

        for (int i = 0; i < player.SizeUp; i++) {
            TransparentBlt(back_hDC, Tail[i].Rx, Tail[i].Ry, 50, 50, player.image.dc, 0, 0, 50, 50, RGB(255, 0, 255));
        }

        // 점수 베너
        BitBlt(back_hDC, 857, 300, 1280, 500, Score_banner.dc, 0, 0, SRCCOPY);

        char str[128];
        sprintf_s(str, "%d", rank.score);
        
        font = CreateFont(100, 50, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "나눔고딕");
        oldfont = (HFONT)SelectObject(back_hDC, font);
        SetBkMode(back_hDC, TRANSPARENT);   // 배경색 투명하게
        TextOut(back_hDC, 1070, 340, str, strlen(str));
        SelectObject(back_hDC, oldfont);
        DeleteObject(font);

        // 전체 맵 그려주기
        BitBlt(g_hDC, 0, 0, WINSIZEX, WINSIZEY, back_hDC, 0, 0, SRCCOPY);

        break;

    // 게임오버 화면
    case 3:

        // 백디씨 흰색으로 덧칠해주기
        Rectangle(back_hDC, -1, -1, WINSIZEX, WINSIZEY);

        BitBlt(back_hDC, 0, 0, WINSIZEX, WINSIZEY, GAMEOVER.dc, 0, 0, SRCCOPY);


        // 닉네임 칸
        Rectangle(back_hDC, 690 + move * 200, 440, 810 + move * 200, 560);

        for (int i = 0; i < 3; i++) {

            char nick[128];

            Rectangle(back_hDC, 700 + i * 200, 450, 800 + i * 200, 550);

            sprintf_s(nick, "%c", rank.nick[i]);

            font = CreateFont(50, 25, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "나눔고딕");
            oldfont = (HFONT)SelectObject(back_hDC, font);
            SetBkMode(back_hDC, TRANSPARENT);   // 배경색 투명하게
            TextOut(back_hDC, 730 + i * 200, 475, nick, strlen(nick));
            SelectObject(back_hDC, oldfont);
            DeleteObject(font);

            
        }

        // 스코어 칸
        char score[3];

        sprintf_s(score, "%d", rank.score);
        
        HFONT font1 = CreateFont(80, 50, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "나눔고딕");
        HFONT oldfont1 = (HFONT)SelectObject(back_hDC, font1);
        SetBkMode(back_hDC, TRANSPARENT);   // 배경색 투명하게
        TextOut(back_hDC, 1050, 270, score, strlen(score));
        SelectObject(back_hDC, oldfont1);
        DeleteObject(font1);

        // 랭킹 칸
        
        for (int i = 0; i < 3; i++) {

            char Rank_W[128];
            sprintf_s(Rank_W, "%s : %d", rank.name, rank.sub_score);

            HFONT font1 = CreateFont(60, 30, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "나눔고딕");
            HFONT oldfont1 = (HFONT)SelectObject(back_hDC, font1);
            SetBkMode(back_hDC, TRANSPARENT);   // 배경색 투명하게
            TextOut(back_hDC, 300, 355, Rank_W, strlen(Rank_W));
            SelectObject(back_hDC, oldfont1);
            DeleteObject(font1);
            }
     


        
        // 마지막에 제발 
        BitBlt(g_hDC, 0, 0, WINSIZEX, WINSIZEY, back_hDC, 0, 0, SRCCOPY);
        
        break;
    }
}

void Release() {

    ReleaseDC(g_hWnd, g_hDC);
    ReleaseDC(g_hWnd, back_hDC);

}
//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKEGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL; //MAKEINTRESOURCEW(IDC_SNAKEGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, WINSIZEX, WINSIZEY, nullptr, nullptr, hInstance, nullptr);

   g_hWnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_MOUSEMOVE:

        xPos = GET_X_LPARAM(lParam);
        yPos = GET_Y_LPARAM(lParam);

            
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}