#include "app_def.h"
#include "mw_tray.h"

int MW_SysTray_Add(MAIN_WINDOW* pMainWnd, LPCTSTR sTip, HICON hIcon)
{
    if (hIcon)
    {
        // Fill the struct only once
        pMainWnd->niSysTray.cbSize           = sizeof(pMainWnd->niSysTray);
        pMainWnd->niSysTray.hWnd             = pMainWnd->hWnd;
        pMainWnd->niSysTray.uID              = SYS_TRAY_ICON_ID;
        pMainWnd->niSysTray.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        pMainWnd->niSysTray.uCallbackMessage = SYS_TRAY_MSG_ID;
        pMainWnd->niSysTray.hIcon            = hIcon;
    }

    if (sTip)
    {
        // Text can depend from situation
        size_t nMaxLength = sizeof(pMainWnd->niSysTray.szTip) / sizeof(TCHAR) - 1;
        StringCchCopy(pMainWnd->niSysTray.szTip, nMaxLength, sTip);
    }

    if (! pMainWnd->bTrayIcon)
    {
        pMainWnd->bTrayIcon = TRUE;
        return (! Shell_NotifyIcon(NIM_ADD, &pMainWnd->niSysTray)) ? EXIT_FAILURE : EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}

int MW_SysTray_Delete(MAIN_WINDOW* pMainWnd)
{
    pMainWnd->bTrayIcon = FALSE;
    return (! Shell_NotifyIcon(NIM_DELETE, &pMainWnd->niSysTray)) ? EXIT_FAILURE : EXIT_SUCCESS;
}
