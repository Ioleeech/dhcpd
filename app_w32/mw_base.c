#include "app_def.h"
#include "mw_base.h"
#include "mw_list.h"
#include "mw_menu.h"
#include "mw_state.h"
#include "mw_tray.h"

static void _MW_Window_Close(HWND hWnd)
{
    MAIN_WINDOW* pMainWnd = (MAIN_WINDOW*) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    // Delete window components
    if (pMainWnd)
    {
        // Delete system tray icon
        MW_SysTray_Delete(pMainWnd);

        // Delete system menu
        MW_SystemMenu_Delete(pMainWnd);
    }

    // Close application
    PostQuitMessage(0);
}

static LRESULT CALLBACK _MW_Window_CmdProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case SYS_TRAY_MSG_ID:
            if (! IsWindowVisible(hWnd))
            {
                switch(lParam)
                {
                    case WM_LBUTTONDBLCLK:
                        ShowWindow(hWnd, SW_RESTORE);
                        break;

                    case WM_RBUTTONDOWN:
                    case WM_CONTEXTMENU:
                        {
                            MAIN_WINDOW* pMainWnd = (MAIN_WINDOW*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
                            MW_SystemMenu_Show(pMainWnd);
                        }
                        break;

                    default:
                        break;
                }
            }
            break;

        case WM_DESTROY: // 0x0002
            _MW_Window_Close(hWnd);
            return 0;

        case WM_SIZE: // 0x0005
            {
                MAIN_WINDOW* pMainWnd = (MAIN_WINDOW*) GetWindowLongPtr(hWnd, GWLP_USERDATA);

                // Update status bar
                if (pMainWnd->hStatus)
                    SendMessage(pMainWnd->hStatus, WM_SIZE, 0, 0);

                // Update address list
                if (pMainWnd->hAddrList)
                {
                    WORD width  = LOWORD(lParam);
                    WORD height = HIWORD(lParam) - pMainWnd->wStatusHeight;

                    MoveWindow(pMainWnd->hAddrList, 0, 0, width, height, TRUE);
                }
            }
            break;

        case WM_CLOSE: // 0x0010
            DestroyWindow(hWnd);
            return 0;

        case WM_COMMAND:    // 0x0111
        case WM_SYSCOMMAND: // 0x0112
            switch(wParam & 0xFFF0)
            {
                case SYSMENU_ITEM_ID_SETUP:
                case SYSMENU_ITEM_ID_START:
                case SYSMENU_ITEM_ID_STOP:
                    // TODO
                    return 0;

                case SYSMENU_ITEM_ID_QUIT:
                    _MW_Window_Close(hWnd);
                    return 0;

                case SC_MINIMIZE: // 0xF020, SC_ICON
                case SC_CLOSE:    // 0xF060
                    ShowWindow(hWnd, SW_HIDE);
                    return 0;

                case SC_MAXIMIZE: // 0xF030, SC_ZOOM
                    ShowWindow(hWnd, SW_MAXIMIZE);
                    return 0;

                case SC_RESTORE: // 0xF120
                    ShowWindow(hWnd, SW_RESTORE);
                    return 0;

                default:
                    break;
            }
            break;

        case WM_SHOWWINDOW: // 0x0018
            if (wParam)
            {
                // Main window is visible, do not show an icon in system tray
                MAIN_WINDOW* pMainWnd = (MAIN_WINDOW*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
                MW_SysTray_Delete(pMainWnd);
            }
            else
            {
                // Main window is hidden, add application icon in system tray
                MAIN_WINDOW* pMainWnd = (MAIN_WINDOW*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
                LPCTSTR      sTrayTip = TEXT(WINDOW_TITLE_TEXT " : " STATUS_TEXT_STOPPED);
                MW_SysTray_Add(pMainWnd, sTrayTip, NULL);
            }
            break;

        default:
            break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int MW_Window_Init(MAIN_WINDOW* pMainWnd, int nCmdShow)
{
    // Prepare resources
    DWORD   dwStyle    = WS_OVERLAPPEDWINDOW;
    LPCTSTR sClassName = TEXT(WINDOW_CLASS_NAME);
    LPCTSTR sWinTitle  = TEXT(WINDOW_TITLE_TEXT);
    LPCTSTR sTrayTip   = TEXT(WINDOW_TITLE_TEXT " : " STATUS_TEXT_STOPPED);
    HICON   hIcon      = (HICON)   LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON,   0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
    HICON   hIconSm    = (HICON)   LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON,   0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
    HCURSOR hCursor    = (HCURSOR) LoadImage(NULL, IDC_ARROW,       IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);
    HBRUSH  hbrBgrnd   = (HBRUSH)  (COLOR_BTNFACE + 1);

    // Register main window class
    WNDCLASSEX wc;
    wc.cbSize        = sizeof(wc);          // UINT
    wc.style         = 0;                   // UINT
    wc.lpfnWndProc   = &_MW_Window_CmdProc; // WNDPROC
    wc.cbClsExtra    = 0;                   // int
    wc.cbWndExtra    = 0;                   // int
    wc.hInstance     = pMainWnd->hInstance; // HINSTANCE
    wc.hIcon         = hIcon;               // HICON
    wc.hCursor       = hCursor;             // HCURSOR
    wc.hbrBackground = hbrBgrnd;            // HBRUSH
    wc.lpszMenuName  = NULL;                // LPCTSTR
    wc.lpszClassName = sClassName;          // LPCTSTR
    wc.hIconSm       = hIconSm;             // HICON

    if (! RegisterClassEx(&wc))
        return EXIT_FAILURE;

    // Create instance of main window
    pMainWnd->hWnd = CreateWindowEx(0,                     // DWORD     dwExStyle
                                    sClassName,            // LPCSTR    lpClassName
                                    sWinTitle,             // LPCSTR    lpWindowName
                                    dwStyle,               // DWORD     dwStyle
                                    CW_USEDEFAULT,         // int       X
                                    CW_USEDEFAULT,         // int       Y
                                    WINDOW_WIDTH_DEFAULT,  // int       nWidth
                                    WINDOW_HEIGHT_DEFAULT, // int       nHeight
                                    NULL,                  // HWND      hWndParent
                                    NULL,                  // HMENU     hMenu
                                    pMainWnd->hInstance,   // HINSTANCE hInstance
                                    NULL);                 // LPVOID    lpParam
    if (! pMainWnd->hWnd)
        return EXIT_FAILURE;

    // Store the handle to base class
    SetWindowLongPtr(pMainWnd->hWnd, GWLP_USERDATA, (LONG_PTR) pMainWnd);

    // Set custom system menu
    if (MW_SystemMenu_Init(pMainWnd) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // Add status bar
    if (MW_StatusBar_Init(pMainWnd) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // Add address list
    if (MW_AddressList_Init(pMainWnd) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // Add system tray icon
    if (MW_SysTray_Add(pMainWnd, sTrayTip, hIcon) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // Show window
    ShowWindow(pMainWnd->hWnd, nCmdShow);
    UpdateWindow(pMainWnd->hWnd);

    return EXIT_SUCCESS;
}
