//HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include<string>
#include <tchar.h>
#include <windowsx.h>

// Global variables
HWND hWnd2;

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szWindowClassChild[] = _T("ChildDesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");
static TCHAR szTitleChild[] = _T("Child Window");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst, hInst2;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProcChild(HWND, UINT, WPARAM, LPARAM);



int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);






    WNDCLASSEX wcex2;

    wcex2.cbSize = sizeof(WNDCLASSEX);
    wcex2.style = CS_HREDRAW | CS_VREDRAW;
    wcex2.lpfnWndProc = WndProcChild;
    wcex2.cbClsExtra = 0;
    wcex2.cbWndExtra = 0;
    wcex2.hInstance = hInstance;
    wcex2.hIcon = LoadIcon(wcex2.hInstance, IDI_APPLICATION);
    wcex2.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex2.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex2.lpszMenuName = NULL;
    wcex2.lpszClassName = szWindowClassChild;
    wcex2.hIconSm = LoadIcon(wcex2.hInstance, IDI_APPLICATION);


    if (!RegisterClassEx(&wcex) || !RegisterClassEx(&wcex2))
    {
        MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Error"), NULL);
        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;
    hInst2 = hInstance;

    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400,
        NULL,
        NULL,
        hInstance,
        NULL
    );

   DWORD dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

    hWnd2 = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClassChild,
        szTitleChild,
        dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        300, 200,
        hWnd,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd || !hWnd2)
    {
        MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Error"), NULL);
        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);


    ShowWindow(hWnd2,
        SW_SHOW);
    UpdateWindow(hWnd2);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // Check if the message is for either the main window or the child window
        if (!IsDialogMessage(hWnd, &msg) && !IsDialogMessage(hWnd2, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    return (int)msg.wParam;
}





//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
//  WM_DESTROY  - post a quit message and return

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static unsigned int glob_counter = 0;
    static unsigned int mouseMoveCount = 0;
    static unsigned int mouseButtonClickCounts[3] = { 0 };

    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_CREATE:
        break;
    case WM_MOUSEMOVE:
        mouseMoveCount++;
        break;
    case WM_LBUTTONDOWN:
    {
        mouseButtonClickCounts[0]++;
        OutputDebugString(_T("Left mouse button clicked (main wndw)\n"));

        // Get client rectangle
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        // Get cursor position
        POINT cursorPos;
        cursorPos.x = GET_X_LPARAM(lParam);
        cursorPos.y = GET_Y_LPARAM(lParam);

        PhysicalToLogicalPoint(hWnd, &cursorPos);

        // Get device context
        HDC hdc = GetDC(hWnd);

        // Create a red pen
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
        // Select the pen into the device context
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        // Create a red brush
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
        // Select the brush into the device context
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        // Draw filled red circle
        Ellipse(hdc, cursorPos.x - 25, cursorPos.y - 25, cursorPos.x + 25, cursorPos.y + 25);

        // Clean up
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPen);
        DeleteObject(hBrush);
        ReleaseDC(hWnd, hdc);
        break;
    }
    case WM_MBUTTONDOWN:
    {
        mouseButtonClickCounts[1]++;
        OutputDebugString(_T("Middle mouse button clicked (main wndw)\n"));
        break;
    }
    case WM_RBUTTONDOWN:
    {
        mouseButtonClickCounts[2]++;
        OutputDebugString(_T("Right mouse button clicked (main wndw)\n"));

        // Clear the client area
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_DESTROY:
    {
        OutputDebugString(_T("Total messages (MAIN WINDOW): "));
        OutputDebugString(std::to_wstring(glob_counter).c_str());
        OutputDebugString(_T("\n"));
        OutputDebugString(_T("Mouse move messages: "));
        OutputDebugString(std::to_wstring(mouseMoveCount).c_str());
        OutputDebugString(_T("\n"));
        OutputDebugString(_T("Left mouse button clicks: "));
        OutputDebugString(std::to_wstring(mouseButtonClickCounts[0]).c_str());
        OutputDebugString(_T("\n"));
        OutputDebugString(_T("Middle mouse button clicks: "));
        OutputDebugString(std::to_wstring(mouseButtonClickCounts[1]).c_str());
        OutputDebugString(_T("\n"));
        OutputDebugString(_T("Right mouse button clicks: "));
        OutputDebugString(std::to_wstring(mouseButtonClickCounts[2]).c_str());
        OutputDebugString(_T("\n"));
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    glob_counter++;

    return 0;
}









LRESULT CALLBACK WndProcChild(HWND hWnd2, UINT message, WPARAM wParam, LPARAM lParam)
{
    static unsigned int glob_counter_chld = 0;
    static unsigned int mouseMoveCount_chld = 0;
    static unsigned int mouseButtonClickCounts_chld[3] = { 0 };

    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("");

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd2, &ps);
        TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
        EndPaint(hWnd2, &ps);
        break;
    case WM_CREATE:

    case WM_MOUSEMOVE:
        mouseMoveCount_chld++;
        break;
    case WM_LBUTTONDOWN:
        mouseButtonClickCounts_chld[0]++;
        OutputDebugString(_T("Left mouse button clicked (chld)\n"));
        break;
    case WM_MBUTTONDOWN:
        mouseButtonClickCounts_chld[1]++;
        OutputDebugString(_T("Middle mouse button clicked (chld)\n"));
        break;
    case WM_RBUTTONDOWN:
        mouseButtonClickCounts_chld[2]++;
        OutputDebugString(_T("Right mouse button clicked (chld)\n"));
        break;
    case WM_DESTROY:
        OutputDebugString(_T("Total messages(CHILD WINDOW): "));
        OutputDebugString(std::to_wstring(glob_counter_chld).c_str());
        OutputDebugString(_T("\n"));
        OutputDebugString(_T("Mouse move messages: "));
        OutputDebugString(std::to_wstring(mouseMoveCount_chld).c_str());
        OutputDebugString(_T("\n"));
        OutputDebugString(_T("Left mouse button clicks: "));
        OutputDebugString(std::to_wstring(mouseButtonClickCounts_chld[0]).c_str());
        OutputDebugString(_T("\n"));
        OutputDebugString(_T("Middle mouse button clicks: "));
        OutputDebugString(std::to_wstring(mouseButtonClickCounts_chld[1]).c_str());
        OutputDebugString(_T("\n"));
        OutputDebugString(_T("Right mouse button clicks: "));
        OutputDebugString(std::to_wstring(mouseButtonClickCounts_chld[2]).c_str());
        OutputDebugString(_T("\n"));
        break;
    default:
        return DefWindowProc(hWnd2, message, wParam, lParam);
        break;
    }

    glob_counter_chld++;

    return 0;
}




