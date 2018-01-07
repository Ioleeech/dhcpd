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
                    printf("wParam = 0x%04X\n", wParam);
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
                MW_SysTray_Add(pMainWnd, NULL, NULL);
            }
            break;

        case WM_CREATE:            // 0x0001
        case WM_MOVE:              // 0x0003
        case WM_ACTIVATE:          // 0x0006
        case WM_SETFOCUS:          // 0x0007
        case WM_KILLFOCUS:         // 0x0008
        case WM_GETTEXT:           // 0x000D
        case WM_PAINT:             // 0x000F
        case WM_QUERYOPEN:         // 0x0013
        case WM_ERASEBKGND:        // 0x0014
        case WM_ACTIVATEAPP:       // 0x001C
        case WM_CANCELMODE:        // 0x001F
        case WM_SETCURSOR:         // 0x0020
        case WM_MOUSEACTIVATE:     // 0x0021
        case WM_GETMINMAXINFO:     // 0x0024
        case WM_GETOBJECT:         // 0x003D
        case WM_WINDOWPOSCHANGING: // 0x0046
        case WM_WINDOWPOSCHANGED:  // 0x0047
        case WM_CONTEXTMENU:       // 0x007B
        case WM_GETICON:           // 0x007F
        case WM_NCCREATE:          // 0x0081
        case WM_NCDESTROY:         // 0x0082
        case WM_NCCALCSIZE:        // 0x0083
        case WM_NCHITTEST:         // 0x0084
        case WM_NCPAINT:           // 0x0085
        case WM_NCACTIVATE:        // 0x0086
        case WM_SYNCPAINT:         // 0x0088
        case WM_NCLBUTTONDBLCLK:   // 0x00A3
        case WM_NCMOUSEMOVE:       // 0x00A0
        case WM_NCLBUTTONDOWN:     // 0x00A1
        case WM_KEYUP:             // 0x0101
        case WM_SYSKEYDOWN:        // 0x0104
        case WM_SYSKEYUP:          // 0x0106
        case WM_INITMENU:          // 0x0116
        case WM_INITMENUPOPUP:     // 0x0117
        case WM_MENUSELECT:        // 0x011F
        case WM_ENTERIDLE:         // 0x0121
        case WM_UNINITMENUPOPUP:   // 0x0125
        case WM_MOUSEFIRST:        // 0x0200
        case WM_LBUTTONDOWN:       // 0x0201
        case WM_LBUTTONUP:         // 0x0202
        case WM_RBUTTONDOWN:       // 0x0204
        case WM_RBUTTONUP:         // 0x0205
        case WM_ENTERMENULOOP:     // 0x0211
        case WM_EXITMENULOOP:      // 0x0212
        case WM_SIZING:            // 0x0214
        case WM_CAPTURECHANGED:    // 0x0215
        case WM_MOVING:            // 0x0216
        case WM_ENTERSIZEMOVE:     // 0x0231
        case WM_EXITSIZEMOVE:      // 0x0232
        case WM_IME_SETCONTEXT:    // 0x0281
        case WM_IME_NOTIFY:        // 0x0282
            break;

        default:
            printf("uMsg = 0x%04X\n", uMsg);
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
    if (MW_SysTray_Add(pMainWnd, sWinTitle, hIcon) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // Show window
    ShowWindow(pMainWnd->hWnd, nCmdShow);
    UpdateWindow(pMainWnd->hWnd);

    return EXIT_SUCCESS;
}
