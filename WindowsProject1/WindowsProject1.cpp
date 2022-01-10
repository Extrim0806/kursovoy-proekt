// WindowsProject1.cpp : Определяет точку входа для приложения.
//
#include "framework.h"
#include "WindowsProject1.h"
#include <fstream>
#include<vector>
#include <commctrl.h>
#include <Codecvt>
#include<postgres_ext.h>
#include <libpq-fe.h>
#include "password.h"
#include <Windows.h>
#include<string>
#include <format>
#include <ctime>
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
HWND hWnd;
HWND button_test;
HWND listview_search;
std::vector <std::vector <wchar_t*> > listw;
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
PGconn* con = PQconnectdb(connect_info);
PGresult* gen = PQexec(con, "select * from library.genres");
PGresult* norm;

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

void ZapCB(HWND CB) {
    int numR = PQntuples(gen);
    int numC = PQnfields(gen);
    for (int i = 0; i < numR; i++){
        const char* per = PQgetvalue(gen, i, 1);
        SendMessageA(CB, CB_ADDSTRING, 0, (LPARAM)per);
    }
};

void ZapLW(HWND listview, std::vector < std::vector<wchar_t*> > spisok) {
    LVITEMA item;
    item.mask = LVIF_TEXT;
    for (int i = 0; i < spisok.size(); i++) {
        for (int j = 0; j < 5; j++) {
            item.iSubItem = j;
            item.iItem = i;
            item.pszText = (LPSTR)spisok[i][j];
            if (j == 0) {
                SendMessage(listview, LVM_INSERTITEM, 0, (LPARAM)&item);
            }
            else {
                SendMessage(listview, LVM_SETITEM, 0, (LPARAM)&item);
            }
            
        }

    }
}

void CleanLW(HWND listview, std::vector < std::vector<wchar_t*> > spisok) {
    LVITEMA item;
    item.mask = LVIF_TEXT;
    for (int i = 0; i < spisok.size(); i++) {
        for (int j = 0; j < 4; j++) {
            SendMessage(listview, LVM_DELETEITEM, 0, 0);

        }

    }
}

void ReadTable(HWND listview,PGresult* res) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    std::vector <std::vector <wchar_t*> > a;
    
    

    
    int numR = PQntuples(res);
    int numC = PQnfields(res);
    const char* per;
    for (int i = 0; i < numR; i++) {
        std::vector<wchar_t*>b;
        for (int j = 1; j < numC; j++) {
            per = PQgetvalue(res, i, j);
            size_t s_res = 0;
            while (per[s_res] != '\0') {
                s_res += 1;
            }
            wchar_t* wch = new wchar_t[s_res];
            size_t siz;
            mbstowcs_s(&siz, wch, s_res + 1, per, s_res);
            
             for (int i = 0; i < s_res; i++) {
                if (wch[i] == 168) {
                    wch[i] = 1025;
                }
                else if (wch[i] == 184) {
                    wch[i] = 1105;
                }
                else if (192 <= wch[i] && wch[i] <= 255) {
                    wch[i] += 848;
                }
             }
            b.push_back(wch);
        }
        a.push_back(b);
    }
    listw = a;
    ZapLW(listview, a);
    
}
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

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   HWND button_search_menu = CreateWindow(L"BUTTON", L"поиск", BS_CENTER|WS_CHILD, 600, 250, 150,50, hWnd, nullptr, hInstance, nullptr);
   HWND button_add_menu = CreateWindow(L"BUTTON", L"добавить", BS_CENTER | WS_CHILD, 600, 350, 150, 50, hWnd, nullptr, hInstance, nullptr);
   HWND button_back = CreateWindow(L"BUTTON", L"<-", BS_CENTER | WS_CHILD, 25, 625, 50, 50, hWnd, nullptr, hInstance, nullptr);
   HWND button_nal = CreateWindow(L"BUTTON", L"есть в наличии", BS_CENTER | WS_CHILD | BS_AUTOCHECKBOX, 1075, 250, 125, 25, hWnd, nullptr, hInstance, nullptr);
   HWND button_create = CreateWindow(L"BUTTON", L"создать", BS_CENTER | WS_CHILD, 1300, 625, 100, 50, hWnd, nullptr, hInstance, nullptr);
   HWND button_search_1 = CreateWindow(L"BUTTON", L"поиск", BS_CENTER | WS_CHILD, 1075, 200, 100, 25, hWnd, nullptr, hInstance, nullptr);
   HWND button_search_2 = CreateWindow(L"BUTTON", L"поиск", BS_CENTER | WS_CHILD, 1075, 300, 100, 25, hWnd, nullptr, hInstance, nullptr);
   HWND button_ras_nastr = CreateWindow(L"BUTTON", L"расш. настройки", BS_CENTER | WS_CHILD, 275, 200, 150, 25, hWnd, nullptr, hInstance, nullptr);
   HWND button_close = CreateWindow(L"BUTTON", L"закрыть", BS_CENTER | WS_CHILD, 175, 300, 100, 25, hWnd, nullptr, hInstance, nullptr);
   //button_test = CreateWindow(L"BUTTON", L"dsfadf", BS_CENTER | WS_CHILD, 1300, 525, 100, 50, hWnd, nullptr, hInstance, nullptr);

   edit_title = CreateWindow(L"EDIT", nullptr, WS_CHILD | ES_LEFT | ES_MULTILINE | WS_BORDER, 175, 100, 1075, 50, hWnd, nullptr, hInstance, nullptr);
   edit_author = CreateWindow(L"EDIT", nullptr, WS_CHILD | ES_LEFT | ES_MULTILINE | WS_BORDER, 175, 175, 1075, 50, hWnd, nullptr, hInstance, nullptr);
   edit_year = CreateWindow(L"EDIT", nullptr, WS_CHILD | ES_LEFT | ES_MULTILINE | WS_BORDER, 175, 250, 100, 25, hWnd, nullptr, hInstance, nullptr);
   edit_nal = CreateWindow(L"EDIT", nullptr, WS_CHILD | ES_LEFT | ES_MULTILINE | WS_BORDER, 1075, 250, 100, 25, hWnd, nullptr, hInstance, nullptr);

   combobox_genre = CreateWindow(L"COMBOBOX", nullptr, CBS_DROPDOWN | WS_CHILD | WS_VSCROLL | WS_VSCROLL, 550, 250, 350, 250, hWnd, nullptr, hInstance, nullptr);

   lable_title = CreateWindow(L"EDIT", L"название", WS_CHILD | ES_LEFT | ES_READONLY | ES_MULTILINE, 175, 75, 125, 25, hWnd, nullptr, hInstance, nullptr);
   lable_author = CreateWindow(L"EDIT", L"автор", WS_CHILD | ES_LEFT | ES_READONLY | ES_MULTILINE, 175, 150, 125, 25, hWnd, nullptr, hInstance, nullptr);
   lable_year = CreateWindow(L"EDIT", L"год", WS_CHILD | ES_LEFT | ES_READONLY | ES_MULTILINE, 175, 225, 100, 25, hWnd, nullptr, hInstance, nullptr);
   lable_nal = CreateWindow(L"EDIT", L"количество", WS_CHILD | ES_LEFT | ES_READONLY | ES_MULTILINE, 1075, 225, 100, 25, hWnd, nullptr, hInstance, nullptr);
   lable_genre = CreateWindow(L"EDIT", L"жанры", WS_CHILD | ES_LEFT | ES_READONLY | ES_MULTILINE, 675, 225, 100, 25, hWnd, nullptr, hInstance, nullptr);

   listview_search = CreateWindowEx(0, WC_LISTVIEW, nullptr, WS_CHILD | WS_BORDER | WS_VSCROLL | LVS_EX_GRIDLINES, 175, 350, 1075, 300, hWnd, nullptr, hInstance, nullptr);

   ZapCB(combobox_genre);

   LV_COLUMN nazw;
   nazw.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
   nazw.cx = 350;
   nazw.fmt = LVCFMT_LEFT;
   nazw.pszText = (LPWSTR)L"название";
   nazw.iSubItem = 0;
   ListView_InsertColumn(listview_search, 0, &nazw);
   nazw.cx = 75;
   nazw.pszText = (LPWSTR)L"год";
   nazw.iSubItem = 1;
   ListView_InsertColumn(listview_search, 1, &nazw);
   nazw.cx = 100;
   nazw.pszText = (LPWSTR)L"количество";
   nazw.iSubItem = 2;
   ListView_InsertColumn(listview_search, 2, &nazw);
   nazw.cx = 150;
   nazw.pszText = (LPWSTR)L"жанр";
   nazw.iSubItem = 3;
   ListView_InsertColumn(listview_search, 3, &nazw);
   nazw.cx = 400;
   nazw.pszText = (LPWSTR)L"автор";
   nazw.iSubItem = 4;
   ListView_InsertColumn(listview_search, 4, &nazw);
   
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

        CleanLW(listview_search,listw);

        SendMessage(edit_title, WM_SETTEXT, 0, (LPARAM)"");
        SendMessage(edit_author, WM_SETTEXT, 0, (LPARAM)"");
        SendMessage(edit_year, WM_SETTEXT, 0, (LPARAM)"");
        SendMessage(edit_nal, WM_SETTEXT, 0, (LPARAM)"");
        SendMessage(combobox_genre, WM_SETTEXT, 0, (LPARAM)"");
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
        norm = PQexec(con, "select t.*, g.name as genre, a.name as author from ( select distinct b.* from ( select * from library.books ) as b ) as t left join library.books_x_authors as bxa on bxa.book_id = t.book_id left join library.author as a on a.author_id = bxa.author_id left join library.books_x_genres as bxg on bxg.book_id = t.book_id left join library.genres as g on g.genre_id = bxg.genre_id;");
        ReadTable(listview_search,norm);
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
        LPSTR Stitle = new char[GetWindowTextLengthA(edit_title)];
        LPSTR Sauthor = new char[GetWindowTextLengthA(edit_author)];
        LPSTR Syear = new char[GetWindowTextLengthA(edit_year)];
        LPSTR Snal = new char[GetWindowTextLengthA(edit_nal)];
        LPSTR Sgenre = new char[GetWindowTextLengthA(combobox_genre)];
        
        GetWindowTextA(edit_title,Stitle,GetWindowTextLengthA(edit_title)+1);
        GetWindowTextA(edit_author, Sauthor, GetWindowTextLengthA(edit_author)+1);
        GetWindowTextA(edit_year, Syear, GetWindowTextLengthA(edit_year)+1);
        GetWindowTextA(edit_nal, Snal, GetWindowTextLengthA(edit_nal)+1);
        GetWindowTextA(combobox_genre, Sgenre, GetWindowTextLengthA(combobox_genre)+1);

        std::string sInsBoo="insert into library.books(name,year,amount) values ('";
        std::string sInsAuth = "insert into library.author(name) values (";
        std::string sInsBxG = "insert into library.books_x_genres(book_id,genre_id) values (";
        std::string sInsBxA = "insert into library.books_x_authors(book_id,author_id) values ";

        std::string sSelGen = "select genre_id from library.genres where name='";
        std::string sSelBoo = "select book_id from library.books where name='";
        std::string sSelAuth = "select author_id from library.author where name in (";
        
        std::string st = (std::string)((char*)Stitle);
        std::string sa = (std::string)((char*)Sauthor);
        std::string sy = (std::string)((char*)Syear);
        std::string sn = (std::string)((char*)Snal);
        std::string sg = (std::string)((char*)Sgenre);
        std::vector<std::string> vsA,vsG;
        std::string sT;
        int dat, nl;
        int indexT = 0, indexA = 0,indexY=0, indexN = 0, indexG = 0;
        int linT = -1, linA = -1,linY=-1, linN = -1, linG = -1;
        
        
        try
        {
            if (st.size() > 0) {

                int c = 0;
                int i = 0;
                while (i < st.size()) {
                    if (st[i] == '/') { throw(L"содержит запр. символ /"); }

                    if (st[i] == '\'') {
                        st = st.substr(0, i) + "\"" + st.substr(i + 1, st.size() - i - 1);
                    }

                    if (st[i] == '-') {
                        c += 1;
                    }
                    else {
                        if (c > 1) {
                            st = st.substr(0, i - c + 1) + st.substr(i, st.size() - i);
                        }
                        i = i - c + 1;
                        c = 0;
                    }
                    i++;
                }
                if (c > 1) {
                    st = st.substr(0, st.size() - c + 1);
                }



                for (int i = 0; i < st.size(); i++) {
                    if (st[i] == ' ' && i == indexT) { indexT += 1; }
                    if (st[i] != ' ') { linT = i; }
                }
                if (indexT <= linT) { sT=(st.substr(indexT, linT - indexT + 1)); }
                else { throw(L""); }
            }
            else { throw(L""); }




            if (sa.size()>0){

                int c = 0;
                int i = 0;
                while (i < sa.size()){
                    if (sa[i] == '/') { throw(L"содержит запр. символ /"); }

                    if (sa[i] == '\'') {
                        sa = sa.substr(0, i) + "\"" + sa.substr(i + 1, sa.size()-i-1);
                    }

                    if (sa[i]=='-') {
                        c += 1;
                    }
                    else { 
                        if (c > 1) {
                            sa = sa.substr(0, i - c + 1) + sa.substr(i, sa.size() - i);
                        }
                        i = i - c + 1;
                        c = 0;
                    }
                    i++;
                }
                if (c > 1) {
                    sa = sa.substr(0, sa.size() - c + 1);
                }

                for (int i = 0; i < sa.size(); i++) {
                    
                    
                    if (sa[i] == ' ' && i == indexA) { indexA += 1; }
                    if (sa[i] == ',') {
                        if (indexA <= linA) { vsA.push_back(sa.substr(indexA, linA - indexA + 1)); }
                        indexA += 1;
                    }
                    else if (sa[i] != ' ') { linA = i; }
                }
                if (indexA <= linA) { vsA.push_back(sa.substr(indexA, linA - indexA + 1)); }
                if (vsA.size() == 0) { throw(L""); }



            }
            else { throw(L""); }
            
            if (sy.size() > 0) {
                if (sy[0] == '0') { throw(L"год не может начинаться с 0"); }
                for (int i = 0; i < sy.size(); i++) {
                    if (!isdigit((unsigned char)sy[i])){ throw(L"год не может содержать другие знаки кроме цифр"); }
                }
                dat = stoi(sy);
                if (dat > 2021) {throw(L"год не может быть больше нынешнего"); }
            }
            else { throw(L""); }
            if (sn.size() > 0) {
                if (sn[0] == '0' && sn.size() > 1) { throw(L"кол-во не может начинаться с 0"); }
                for (int i = 0; i < sn.size(); i++) {
                    if (!isdigit((unsigned char)sn[i])) { throw(L"кол-во не может содержать другие знаки кроме цифр"); }
                }
                nl = stoi(sn);
            }
            else { throw(L""); }

            bool chg = false;

            for (int i = 0; i < PQntuples(gen); i++){
                if (PQgetvalue(gen, i, 1) == sg) { chg = true; }
            }
            if (!chg) { throw(L"жанр не находится в списке"); }

            PGresult* id_SelGen = PQexec(con, (sSelGen + sg + "'").c_str());
            
            //=====================
            for (int i = 0; i < vsA.size()-1; i++) {
                sSelAuth += "'" + vsA[i] + "',";
            }
            std::vector<int>aid;
            sSelAuth += "'" + vsA[vsA.size()-1] + "');";
            PGresult* id_SelAuth = PQexec(con,(sSelAuth).c_str());
            std::vector<std::string> net;
            for (int i = 0; i < vsA.size(); i++) {
                bool a = false;
                for (int j = 0; j < PQntuples(id_SelAuth); j++) {
                    if (vsA[i] == PQgetvalue(id_SelAuth, j, 1)) {
                        a = true;
                        aid.push_back(std::stoi(PQgetvalue(id_SelAuth, j, 0)));
                        break;
                    }
                }
                if (!a) { net.push_back(vsA[i]); }
            }
            if (net.size()>0){
                for (int i = 0; i < net.size()-1; i++){
                    sInsAuth += "'" + net[i] + "',";
                }
                sInsAuth += "'" + net[net.size()-1] + "') returning author_id;";
            }
            PGresult* id_SelAuth2=PQexec(con, (sInsAuth).c_str());
            for (int i = 0; i < PQntuples(id_SelAuth2);i++) {
                aid.push_back(std::stoi(PQgetvalue(id_SelAuth2, i, 0)));
            }
            PGresult* id_SelBoo = PQexec(con, (sInsBoo + st + "','" + sy + "'," + sn + ") returning book_id").c_str());
            PQexec(con, (sInsBxG + PQgetvalue(id_SelBoo, 0, 0) + "," + PQgetvalue(id_SelGen, 0, 0) + ")").c_str());
            for (int i = 0; i < aid.size() - 1; i++) {
                sInsBxA += std::string("(") + PQgetvalue(id_SelBoo, 0, 0) + "," + std::to_string(aid[i]) + "),";
            }
            sInsBxA += std::string("(") + PQgetvalue(id_SelBoo, 0, 0) + "," + std::to_string(aid[aid.size() - 1]) + ");";
            PQexec(con, (sInsBxA).c_str());
        }
        catch (const wchar_t* e)
        {
            int msgboxID = MessageBox(
                NULL,
                e,
                L"Confirm Save As",
                MB_ICONSTOP | MB_OK
            );
        }
        
    }
    if ((hWnd == spisButton[5]) || (hWnd == spisButton[8])) {
        LRESULT rn = SendMessage(spisButton[3], BM_GETCHECK,0,0);//BST_CHECKED или BST_UNCHECKED

        LPSTR Stitle = new char[GetWindowTextLengthA(edit_title)];
        LPSTR Sauthor = new char[GetWindowTextLengthA(edit_author)];
        LPSTR Syear = new char[GetWindowTextLengthA(edit_year)];
        LPSTR Sgenre = new char[GetWindowTextLengthA(combobox_genre)];

        GetWindowTextA(edit_title, Stitle, GetWindowTextLengthA(edit_title) + 1);
        GetWindowTextA(edit_author, Sauthor, GetWindowTextLengthA(edit_author) + 1);
        GetWindowTextA(edit_year, Syear, GetWindowTextLengthA(edit_year) + 1);
        GetWindowTextA(combobox_genre, Sgenre, GetWindowTextLengthA(combobox_genre) + 1);

        std::string st = (std::string)((char*)Stitle);
        std::string sa = (std::string)((char*)Sauthor);
        std::string sy = (std::string)((char*)Syear);
        std::string sg = (std::string)((char*)Sgenre);

        std::vector<std::string> vsA, vsG;
        std::string sT;
        int dat=-1;
        int indexT = 0, indexA = 0, indexY = 0, indexG = 0;
        int linT = -1, linA = -1, linY = -1, linG = -1;

        std::string Sel="select t.*, g.name as genre, a.name as author from ( select distinct b.* from  ( select * from library.books ";
        try{
            if (st.size()>0 || sy.size() > 0 || rn == BST_CHECKED) {
                Sel += "where ";
                if (st.size() > 0) {

                    int c = 0;
                    int i = 0;
                    while (i < st.size()) {
                        if (st[i] == '/') { throw(L"содержит запр. символ /"); }

                        if (st[i] == '\'') {
                            st = st.substr(0, i) + "\"" + st.substr(i + 1, st.size() - i - 1);
                        }

                        if (st[i] == '-') {
                            c += 1;
                        }
                        else {
                            if (c > 1) {
                                st = st.substr(0, i - c + 1) + st.substr(i, st.size() - i);
                            }
                            i = i - c + 1;
                            c = 0;
                        }
                        i++;
                    }
                    if (c > 1) {
                        st = st.substr(0, st.size() - c + 1);
                    }

                    for (int i = 0; i < st.size(); i++) {
                        if (st[i] == ' ' && i == indexT) { indexT += 1; }
                        if (st[i] != ' ') { linT = i; }
                    }
                    if (indexT <= linT) { sT = (st.substr(indexT, linT - indexT + 1)); }
                    if (sT.size() > 0) { Sel += "name ='" + sT + "' "; }
                    
                }
                
                if (sy.size() > 0) {
                    if (sy[0] == '0') { throw(L"год не может начинаться с 0"); }
                    for (int i = 0; i < sy.size(); i++) {
                        if (!isdigit((unsigned char)sy[i])) { throw(L"год не может содержать другие знаки кроме цифр"); }
                    }
                    dat = stoi(sy);
                    if (dat > 2021) { throw(L"год не может быть больше нынешнего"); }
                    if (sT.size() > 0 && dat>0) {
                        Sel += "and ";
                    }
                    if (dat > 0) { Sel += "year=" + sy; }
                    
                }
               
                if (rn == BST_CHECKED) {
                    if (dat > 0) {
                        Sel += " and ";
                    }
                    Sel += "amount > 0";
                }
            }
            Sel += ") as b ";


            bool chg = false;

            for (int i = 0; i < PQntuples(gen); i++) {
                if (PQgetvalue(gen, i, 1) == sg) { chg = true; }
            }
            if (chg == false && sg.size() > 0) { throw(L""); }
            
            if (sg.size() > 0) {
                Sel += "left join library.books_x_genres as bxg on bxg.book_id = b.book_id inner join library.genres as g on g.genre_id = bxg.genre_id and g.name = '" + sg + "' ";
            }

          
            
            if (sa.size() > 0) {
                int c = 0;
                int i = 0;
                while (i < sa.size()){
                    if (sa[i] == '/') { throw(L"содержит запр. символ /"); }

                    if (sa[i] == '\'') {
                        sa = sa.substr(0, i) + "\"" + sa.substr(i + 1, sa.size()-i-1);
                    }

                    if (sa[i]=='-') {
                        c += 1;
                    }
                    else { 
                        if (c > 1) {
                            sa = sa.substr(0, i-c+1) + sa.substr(i, sa.size() - i);
                        }
                        i = i - c + 1;
                        c = 0;
                    }
                    i++;
                }
                if (c > 1) {
                    sa = sa.substr(0, sa.size() - c + 1);
                }


                for (int i = 0; i < sa.size(); i++) {
                    if (sa[i] == ' ' && i == indexA) { indexA += 1; }
                    if (sa[i] == ',') {
                        if (indexA <= linA) { vsA.push_back(sa.substr(indexA, linA - indexA + 1)); }
                        indexA += 1;
                    }
                    else if (sa[i] != ' ') { linA = i; }
                }
                if (indexA <= linA) { vsA.push_back(sa.substr(indexA, linA - indexA + 1)); }
                if (vsA.size() > 0) {
                    Sel += "left join library.books_x_authors as bxa on bxa.book_id = b.book_id inner join library.author as a on bxa.author_id = a.author_id and (a.name like '%" + vsA[0] + "%' ";
                }
                if (vsA.size()>1){
                    for (int i = 1; i < vsA.size(); i++){
                        Sel += "or a.name like '%" + vsA[i] + "%' ";
                    }
                }
                Sel += ")";
                
            }
            Sel += ") as t left join library.books_x_authors as bxa on bxa.book_id = t.book_id left join library.author as a on a.author_id = bxa.author_id left join library.books_x_genres as bxg on bxg.book_id = t.book_id left join library.genres as g on g.genre_id = bxg.genre_id;";
            
            PGresult* res=PQexec(con, (Sel).c_str());
            CleanLW(listview_search, listw);
            ReadTable(listview_search,res);
        }
        catch (const wchar_t* e){
            int msgboxID = MessageBox(
                NULL,
                e,
                L"Confirm Save As",
                MB_ICONSTOP | MB_OK
            );
        }
       
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
