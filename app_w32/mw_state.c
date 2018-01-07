#include "app_def.h"
#include "mw_state.h"

int MW_StatusBar_Init(MAIN_WINDOW* pMainWnd)
{
    DWORD dwStyle = WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP;

    if (! pMainWnd->hWnd)
        return EXIT_FAILURE;

    // Create instance of status bar as a child window
    pMainWnd->hStatus = CreateWindowEx(0,                     // DWORD     dwExStyle
                                       STATUSCLASSNAME,       // LPCSTR    lpClassName
                                       NULL,                  // LPCSTR    lpWindowName
                                       dwStyle,               // DWORD     dwStyle
                                       0,                     // int       X
                                       0,                     // int       Y
                                       0,                     // int       nWidth
                                       0,                     // int       nHeight
                                       pMainWnd->hWnd,        // HWND      hWndParent
                                       (HMENU) STATUS_BAR_ID, // HMENU     hMenu
                                       pMainWnd->hInstance,   // HINSTANCE hInstance
                                       NULL);                 // LPVOID    lpParam
    if (! pMainWnd->hStatus)
        return EXIT_FAILURE;

    // Store the height of status bar (it is constant)
    RECT rc = { 0 };
    GetClientRect(pMainWnd->hStatus, &rc);

    pMainWnd->wStatusHeight = rc.bottom;

    // Print out server status
    MW_StatusBar_UpdateState(pMainWnd, FALSE);

    return EXIT_SUCCESS;
}

void MW_StatusBar_UpdateState(MAIN_WINDOW* pMainWnd, BOOL bStarted)
{
    if (pMainWnd->hStatus)
    {
        if (bStarted)
        {
            LPCTSTR sStatusText = TEXT(STATUS_TEXT_STARTED);
            SendMessage(pMainWnd->hStatus, SB_SETTEXT, 0, (LPARAM) sStatusText);
        }
        else
        {
            LPCTSTR sStatusText = TEXT(STATUS_TEXT_STOPPED);
            SendMessage(pMainWnd->hStatus, SB_SETTEXT, 0, (LPARAM) sStatusText);
        }
    }
}
