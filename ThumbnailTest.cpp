// ThumbnailTest.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "ThumbnailTest.h"
#include "Resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szMainClass[MAX_LOADSTRING];              // the main window class name
WCHAR szCounterClass[MAX_LOADSTRING];           // the counter window class name
WCHAR szThumbnailClass[MAX_LOADSTRING];         // the thumbnail window class name
HWND  hwndLastCounter = NULL;
float dpiFactor = 1;

// Forward declarations of functions included in this code module:
BOOL                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    CounterWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ThumbnailWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    dpiFactor = GetDpiForSystem() / 96.0f;

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_THUMBNAILTEST, szMainClass, MAX_LOADSTRING);
    swprintf_s(szCounterClass, ARRAYSIZE(szCounterClass), L"%sCounter", szMainClass);
    swprintf_s(szThumbnailClass, ARRAYSIZE(szThumbnailClass), L"%sThumbnail", szMainClass);

    if (!MyRegisterClass(hInstance))
    {
        return FALSE;
    }

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THUMBNAILTEST));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
BOOL MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = MainWndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 8;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THUMBNAILTEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_THUMBNAILTEST);
    wcex.lpszClassName  = szMainClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    if (!RegisterClassExW(&wcex))
    {
        return FALSE;
    }

    wcex.lpfnWndProc = CounterWndProc;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szCounterClass;

    if (!RegisterClassExW(&wcex))
    {
        return FALSE;
    }

    wcex.lpfnWndProc = ThumbnailWndProc;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szThumbnailClass;

    if (!RegisterClassExW(&wcex))
    {
        return FALSE;
    }

    return TRUE;
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szMainClass, szTitle, WS_OVERLAPPEDWINDOW, (int)(600 * dpiFactor), (int)(100 * dpiFactor), (int)(300 * dpiFactor), (int)(300 * dpiFactor),
       nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void CreateCounterWindow(DWORD dwStyle, HWND hwndParent)
{
    hwndLastCounter = CreateWindowExW(0, szCounterClass, L"Counter", dwStyle | WS_VISIBLE,
        (int)(100 * dpiFactor), (int)(100 * dpiFactor), (int)(300 * dpiFactor), (int)(300 * dpiFactor),
        hwndParent, nullptr, hInst, nullptr);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case ID_FILE_USECAPTION:
                CreateCounterWindow(WS_OVERLAPPEDWINDOW, hWnd);
                break;
            case ID_FILE_NOCAPTION:
                // Note that adding WS_BORDER or WS_DLGFRAME here would make issue not reproducible
                CreateCounterWindow(WS_POPUP, hWnd);
                break;
            case ID_FILE_THUMBNAIL:
                if (!hwndLastCounter)
                {
                    MessageBox(hWnd, L"Create source window first", NULL, MB_OK);
                }
                else
                {
                    // Thumbnail window is created below source window
                    RECT r;
                    GetWindowRect(hwndLastCounter, &r);
                    HWND hwndThumbnail = CreateWindowExW(WS_EX_LAYERED, szThumbnailClass, L"Thumbnail", WS_POPUP | WS_VISIBLE, r.left, r.bottom + 20, r.right - r.left, r.bottom - r.top,
                        nullptr, nullptr, hInst, nullptr);
                }
                break;
            case ID_FILE_MOVECOUNTER:
                if (hwndLastCounter)
                {
                    // Moving source window programmatically also reproduces issue but it seems
                    // only after the second move
                    RECT r;
                    GetWindowRect(hwndLastCounter, &r);
                    r.left += 60;
                    SetWindowPos(hwndLastCounter, NULL, r.left, r.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                }
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
            BeginPaint(hWnd, &ps);
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

void OnCounterPaint(HWND hWnd)
{
    static HFONT hFontCounter = NULL;
    TCHAR s[20];
    SYSTEMTIME st;
    GetLocalTime(&st);
    int n = wsprintf(s, L"%02d:%02d", st.wMinute, st.wSecond);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    RECT rect;
    HBRUSH hbr = NULL;
    GetClientRect(hWnd, &rect);
    hbr = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
    FillRect(hdc, &rect, hbr);
    DeleteObject(hbr);

    if (hFontCounter == NULL)
    {
        hFontCounter = CreateFont(-120, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Tahoma");
    }

    int oldBkMode = SetBkMode(hdc, TRANSPARENT);
    HGDIOBJ oldFont = SelectObject(hdc, hFontCounter);
    SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
    DrawText(hdc, s, -1, &rect, DT_CENTER | DT_VCENTER);
    SelectObject(hdc, oldFont);
    SetBkMode(hdc, oldBkMode);

    EndPaint(hWnd, &ps);
}

LRESULT CALLBACK CounterWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        OnCounterPaint(hWnd);
        break;
    case WM_CREATE:
        SetTimer(hWnd, 1, 1000, NULL);
        break;
    case WM_TIMER:
        if (wParam == 1)
        {
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    case WM_NCHITTEST:
    {
        LRESULT res = DefWindowProc(hWnd, message, wParam, lParam);

        // This allows moving the window when caption is not visible
        if (res == HTCLIENT)
            res = HTCAPTION;
        return res;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void SetupThumbnail(HWND hWndTarget, HWND hwndSource)
{
    TCHAR s[128];
    HTHUMBNAIL hThumbnail = NULL;
    HRESULT hres = DwmRegisterThumbnail(hWndTarget, hwndSource, &hThumbnail);
    if (FAILED(hres))
    {
        swprintf_s(s, ARRAYSIZE(s), L"RegisterThumbnail failed %X", hres);
        MessageBox(NULL, s, NULL, MB_OK);
        return;
    }

    SetWindowLongPtr(hWndTarget, 0, (LONG_PTR)hThumbnail);

    // The destination rectangle size
    RECT r;
    GetWindowRect(hwndSource, &r);
    OffsetRect(&r, -r.left, -r.top);

    // Set the thumbnail properties for use
    DWM_THUMBNAIL_PROPERTIES dskThumbProps{};
    dskThumbProps.dwFlags = DWM_TNP_RECTDESTINATION | DWM_TNP_VISIBLE | DWM_TNP_SOURCECLIENTAREAONLY;

    // Use the window frame and client area
    dskThumbProps.fSourceClientAreaOnly = FALSE;
    dskThumbProps.fVisible = TRUE;
    dskThumbProps.rcDestination = r;

    // Display the thumbnail
    hres = DwmUpdateThumbnailProperties(hThumbnail, &dskThumbProps);
    if (FAILED(hres))
    {
        swprintf_s(s, ARRAYSIZE(s), L"UpdateThumbnail failed %X", hres);
        MessageBox(NULL, s, NULL, MB_OK);
        return;
    }
}

LRESULT CALLBACK ThumbnailWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        SetupThumbnail(hWnd, hwndLastCounter);
        break;
    case WM_DESTROY:
        {
            LONG_PTR hThumbnail = GetWindowLong(hWnd, 0);
            if (hThumbnail)
            {
                DwmUnregisterThumbnail((HTHUMBNAIL)hThumbnail);
            }
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Message handler for about box.
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

