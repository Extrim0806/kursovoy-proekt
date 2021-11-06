// WindowsProject1.cpp : Определяет точку входа для приложения.
//
#include "framework.h"
#include "WindowsProject1.h"
#include <fstream>
#include<vector>
#include <commctrl.h>

#pragma comment(lib,"Comctl32.lib")
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
std::vector<HWND> spisButton;
HWND combobox_genre;
HWND edit_title, edit_author, edit_year, edit_nal;
HWND lable_title, lable_author, lable_year,lable_nal, lable_genre;
HWND listview_search;
/*
spisButton
0. button_back
1. button_search_menu
2. button_add_menu
3. button_nal
4. button_create
5. button_search_1
6. button_ras_nastr
7. button_close
8. button_search_2
*/
// Отправить объявления функций, включенных в этот модуль кода:
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

    // TODO: Разместите код здесь.


    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

void ZapCB(HWND CB,const char* a) {
    std::ifstream F(a);
    char buff[50];
    while (!F.eof()) {
        F.getline(buff, 50);
        SendMessageA(CB, CB_ADDSTRING, 0, (LPARAM)buff);
    }
    F.close();
};
//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   HWND button_search_menu = CreateWindow(L"BUTTON", L"SEARCH", BS_CENTER|WS_CHILD, 600, 250, 150,50, hWnd, nullptr, hInstance, nullptr);
   HWND button_add_menu = CreateWindow(L"BUTTON", L"ADD", BS_CENTER | WS_CHILD, 600, 350, 150, 50, hWnd, nullptr, hInstance, nullptr);
   HWND button_back = CreateWindow(L"BUTTON", L"<-", BS_CENTER | WS_CHILD, 25, 625, 50, 50, hWnd, nullptr, hInstance, nullptr);
   HWND button_nal = CreateWindow(L"BUTTON", L"Nal", BS_CENTER | WS_CHILD | BS_AUTOCHECKBOX, 1075, 250, 100, 25, hWnd, nullptr, hInstance, nullptr);
   HWND button_create = CreateWindow(L"BUTTON", L"CREATE", BS_CENTER | WS_CHILD, 1300, 625, 100, 50, hWnd, nullptr, hInstance, nullptr);
   HWND button_search_1 = CreateWindow(L"BUTTON", L"SEARCH", BS_CENTER | WS_CHILD, 1075, 200, 100, 25, hWnd, nullptr, hInstance, nullptr);
   HWND button_search_2 = CreateWindow(L"BUTTON", L"SEARCH", BS_CENTER | WS_CHILD, 1075, 300, 100, 25, hWnd, nullptr, hInstance, nullptr);
   HWND button_ras_nastr = CreateWindow(L"BUTTON", L"RAS_NASTR", BS_CENTER | WS_CHILD, 275, 200, 150, 25, hWnd, nullptr, hInstance, nullptr);
   HWND button_close = CreateWindow(L"BUTTON", L"CLOSE", BS_CENTER | WS_CHILD, 175, 300, 100, 25, hWnd, nullptr, hInstance, nullptr);

   edit_title = CreateWindow(L"EDIT", nullptr, WS_CHILD | ES_LEFT | ES_MULTILINE | WS_BORDER, 175, 100, 1075, 50, hWnd, nullptr, hInstance, nullptr);
   edit_author = CreateWindow(L"EDIT", nullptr, WS_CHILD | ES_LEFT | ES_MULTILINE | WS_BORDER, 175, 175, 1075, 50, hWnd, nullptr, hInstance, nullptr);
   edit_year = CreateWindow(L"EDIT", nullptr, WS_CHILD | ES_LEFT | ES_MULTILINE | WS_BORDER, 175, 250, 100, 25, hWnd, nullptr, hInstance, nullptr);
   edit_nal = CreateWindow(L"EDIT", nullptr, WS_CHILD | ES_LEFT | ES_MULTILINE | WS_BORDER, 1075, 250, 100, 25, hWnd, nullptr, hInstance, nullptr);

   combobox_genre = CreateWindow(L"COMBOBOX", nullptr, CBS_DROPDOWN | WS_CHILD | WS_VSCROLL, 675, 250, 100, 250, hWnd, nullptr, hInstance, nullptr);

   lable_title = CreateWindow(L"EDIT", L"TITLE", WS_CHILD | ES_LEFT | ES_READONLY | ES_MULTILINE, 175, 75, 125, 25, hWnd, nullptr, hInstance, nullptr);
   lable_author = CreateWindow(L"EDIT", L"AUTHOR", WS_CHILD | ES_LEFT | ES_READONLY | ES_MULTILINE, 175, 150, 125, 25, hWnd, nullptr, hInstance, nullptr);
   lable_year = CreateWindow(L"EDIT", L"YEAR", WS_CHILD | ES_LEFT | ES_READONLY | ES_MULTILINE, 175, 225, 100, 25, hWnd, nullptr, hInstance, nullptr);
   lable_nal = CreateWindow(L"EDIT", L"KOLICHESTVO", WS_CHILD | ES_LEFT | ES_READONLY | ES_MULTILINE, 1075, 225, 100, 25, hWnd, nullptr, hInstance, nullptr);
   lable_genre = CreateWindow(L"EDIT", L"GENRE", WS_CHILD | ES_LEFT | ES_READONLY | ES_MULTILINE, 675, 225, 100, 25, hWnd, nullptr, hInstance, nullptr);

   listview_search = CreateWindowEx(0, WC_LISTVIEW, nullptr, WS_CHILD | WS_BORDER | WS_VSCROLL | LVS_EX_GRIDLINES, 175, 350, 1075, 300, hWnd, nullptr, hInstance, nullptr);

   //ZapCB(combobox_genre,"genre.txt");

   LV_COLUMN nazw;
   nazw.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
   nazw.cx = 200;
   nazw.fmt = LVCFMT_LEFT;
   nazw.pszText = (LPWSTR)L"nazwanie";
   nazw.iSubItem = 0;
   ListView_InsertColumn(listview_search, 0, &nazw);
   nazw.pszText = (LPWSTR)L"avtor";
   nazw.iSubItem = 1;
   ListView_InsertColumn(listview_search, 1, &nazw);


   spisButton.push_back(button_back);
   spisButton.push_back(button_search_menu);
   spisButton.push_back(button_add_menu);
   spisButton.push_back(button_nal);
   spisButton.push_back(button_create);
   spisButton.push_back(button_search_1);
   spisButton.push_back(button_ras_nastr);
   spisButton.push_back(button_close);
   spisButton.push_back(button_search_2);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   ShowWindow(button_search_menu, nCmdShow);
   ShowWindow(button_add_menu, nCmdShow);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
void IdentButton(HWND hWnd) {//hWnd-идентификатор кнопки
    if (hWnd==spisButton[0]){//back
        ShowWindow(spisButton[1], SW_SHOW);
        ShowWindow(spisButton[2], SW_SHOW);

        ShowWindow(spisButton[0], SW_HIDE);
        ShowWindow(spisButton[3], SW_HIDE);
        ShowWindow(spisButton[4], SW_HIDE);
        ShowWindow(spisButton[5], SW_HIDE);
        ShowWindow(spisButton[6], SW_HIDE);
        ShowWindow(spisButton[7], SW_HIDE);
        ShowWindow(spisButton[8], SW_HIDE);

        ShowWindow(edit_title, SW_HIDE);
        ShowWindow(lable_title, SW_HIDE);
        ShowWindow(edit_author, SW_HIDE);
        ShowWindow(lable_author, SW_HIDE);
        ShowWindow(edit_year, SW_HIDE);
        ShowWindow(lable_year, SW_HIDE);
        ShowWindow(edit_nal, SW_HIDE);
        ShowWindow(lable_nal, SW_HIDE);

        ShowWindow(combobox_genre, SW_HIDE);
        ShowWindow(lable_genre, SW_HIDE);

        ShowWindow(listview_search, SW_HIDE);
    }
    if (hWnd == spisButton[1]) {//search_menu
        ShowWindow(spisButton[1], SW_HIDE);
        ShowWindow(spisButton[2], SW_HIDE);

        ShowWindow(spisButton[0], SW_SHOW);
        ShowWindow(spisButton[5], SW_SHOW);
        ShowWindow(spisButton[6], SW_SHOW);

        ShowWindow(edit_title, SW_SHOW);
        ShowWindow(lable_title, SW_SHOW);

        ShowWindow(listview_search, SW_SHOW);
    }
    if (hWnd == spisButton[2]) {//add_menu
        ShowWindow(spisButton[1], SW_HIDE);
        ShowWindow(spisButton[2], SW_HIDE);

        ShowWindow(spisButton[0], SW_SHOW);
        ShowWindow(spisButton[4], SW_SHOW);

        ShowWindow(edit_title, SW_SHOW);
        ShowWindow(lable_title, SW_SHOW);
        ShowWindow(edit_author, SW_SHOW);
        ShowWindow(lable_author, SW_SHOW);
        ShowWindow(edit_year, SW_SHOW);
        ShowWindow(lable_year, SW_SHOW);
        ShowWindow(edit_nal, SW_SHOW);
        ShowWindow(lable_nal, SW_SHOW);

        ShowWindow(combobox_genre, SW_SHOW);
        ShowWindow(lable_genre, SW_SHOW);
    }
    if (hWnd == spisButton[6]) {//ras_nastr
        ShowWindow(spisButton[5], SW_HIDE);
        ShowWindow(spisButton[6], SW_HIDE);

        ShowWindow(spisButton[3], SW_SHOW);
        ShowWindow(spisButton[7], SW_SHOW);
        ShowWindow(spisButton[8], SW_SHOW);

        ShowWindow(edit_author, SW_SHOW);
        ShowWindow(lable_author, SW_SHOW);
        ShowWindow(edit_year, SW_SHOW);
        ShowWindow(lable_year, SW_SHOW);

        ShowWindow(combobox_genre, SW_SHOW);
        ShowWindow(lable_genre, SW_SHOW);
    }
    if (hWnd == spisButton[4]) {//create
        IdentButton(spisButton[0]);
    }
    if (hWnd==spisButton[7]){//close
        ShowWindow(spisButton[3], SW_HIDE);
        ShowWindow(spisButton[7], SW_HIDE);
        ShowWindow(spisButton[8], SW_HIDE);


        ShowWindow(spisButton[5], SW_SHOW);
        ShowWindow(spisButton[6], SW_SHOW);

        ShowWindow(edit_author, SW_HIDE);
        ShowWindow(lable_author, SW_HIDE);
        ShowWindow(edit_year, SW_HIDE);
        ShowWindow(lable_year, SW_HIDE);

        ShowWindow(combobox_genre, SW_HIDE);
        ShowWindow(lable_genre, SW_HIDE);
    }
    
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case BN_CLICKED:
                IdentButton((HWND)lParam);
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
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
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

// Обработчик сообщений для окна "О программе".
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
