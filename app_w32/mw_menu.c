#include "app_def.h"
#include "mw_menu.h"

int MW_SystemMenu_Init(MAIN_WINDOW* pMainWnd)
{
    if (! pMainWnd->hWnd)
        return EXIT_FAILURE;

    // Get handler to system menu
    pMainWnd->hSysMenu = GetSystemMenu(pMainWnd->hWnd, FALSE);

    if (! pMainWnd->hSysMenu)
        return EXIT_FAILURE;

    // Create popup menu
    pMainWnd->hTrayMenu = CreatePopupMenu();

    if (! pMainWnd->hTrayMenu)
        return EXIT_FAILURE;

    // There is no default menu item
    SetMenuDefaultItem(pMainWnd->hSysMenu, -1, TRUE);

    // Parse system menu:
    // - Remove allready existed separators
    // - Copy items to popup menu
    int i = 0;

    for ( ; ; )
    {
        // Check for the end of list
        int i_max = GetMenuItemCount(pMainWnd->hSysMenu);

        if (i >= i_max)
            break;

        // Get item ID
        UINT wID = GetMenuItemID(pMainWnd->hSysMenu, i);

        if ((wID) && (wID != SC_SEPARATOR))
        {
            // Get item text
            TCHAR sItemText [64] = { 0 };
            int   nTextLength = GetMenuString(pMainWnd->hSysMenu, i, sItemText, (64 - 1), MF_BYPOSITION);

            // Get item bitmap (icon)
            MENUITEMINFO miiBMP = { 0 };
            miiBMP.cbSize   = sizeof(miiBMP);
            miiBMP.fMask    = MIIM_FTYPE | MIIM_BITMAP;
            miiBMP.fType    = MFT_BITMAP;
            miiBMP.hbmpItem = NULL;

            GetMenuItemInfo(pMainWnd->hSysMenu, i, TRUE, &miiBMP);

            // Copy item to popup menu
            int nItemPos = GetMenuItemCount(pMainWnd->hTrayMenu);

            MENUITEMINFO mii = { 0 };
            mii.cbSize     = sizeof(mii);
            mii.fMask      = MIIM_FTYPE | MIIM_ID | MIIM_STRING | MIIM_BITMAP;
            mii.fType      = MFT_STRING;
            mii.wID        = wID;
            mii.dwTypeData = (LPTSTR) sItemText;
            mii.cch        = nTextLength;
            mii.hbmpItem   = miiBMP.hbmpItem;

            InsertMenuItem(pMainWnd->hTrayMenu, nItemPos, TRUE, &mii);

            // Only "Restore" item is enabled
            if (wID != SC_RESTORE)
                EnableMenuItem(pMainWnd->hTrayMenu, nItemPos, MF_BYPOSITION | MF_GRAYED);
            else
                SetMenuDefaultItem(pMainWnd->hTrayMenu, nItemPos, TRUE);

            // Go to next menu item
            i ++;
            continue;
        }

        // Remove separator
        RemoveMenu(pMainWnd->hSysMenu, i, MF_BYPOSITION);
    }

    // Extend menu by new items
    LPCTSTR sItemSetup = TEXT(SYSMENU_TEXT_SETUP);
    LPCTSTR sItemStart = TEXT(SYSMENU_TEXT_START);
    LPCTSTR sItemStop  = TEXT(SYSMENU_TEXT_STOP);
    LPCTSTR sItemQuit  = TEXT(SYSMENU_TEXT_QUIT);

    // The same struct can be used for all menu items
    MENUITEMINFO mii = { 0 };
    mii.cbSize   = sizeof(mii);
    mii.fMask    = MIIM_FTYPE | MIIM_ID | MIIM_STRING;
    mii.fType    = MFT_STRING;

    // Another one struct will be used for separators only
    MENUITEMINFO separator = { 0 };
    separator.cbSize = sizeof(separator);
    separator.fMask  = MIIM_FTYPE;
    separator.fType  = MFT_SEPARATOR;

    // Add separator at the end of list to divide system commands
    InsertMenuItem(pMainWnd->hSysMenu,  GetMenuItemCount(pMainWnd->hSysMenu),  TRUE, &separator);
    InsertMenuItem(pMainWnd->hTrayMenu, GetMenuItemCount(pMainWnd->hTrayMenu), TRUE, &separator);

    // Add menu item: configure
    mii.wID        = SYSMENU_ITEM_ID_SETUP;
    mii.dwTypeData = (LPTSTR) sItemSetup;
    mii.cch        = lstrlen(sItemSetup);

    InsertMenuItem(pMainWnd->hSysMenu,  GetMenuItemCount(pMainWnd->hSysMenu),  TRUE, &mii);
    InsertMenuItem(pMainWnd->hTrayMenu, GetMenuItemCount(pMainWnd->hTrayMenu), TRUE, &mii);

    // Add menu item: start
    mii.wID        = SYSMENU_ITEM_ID_START;
    mii.dwTypeData = (LPTSTR) sItemStart;
    mii.cch        = lstrlen(sItemStart);

    InsertMenuItem(pMainWnd->hSysMenu,  GetMenuItemCount(pMainWnd->hSysMenu),  TRUE, &mii);
    InsertMenuItem(pMainWnd->hTrayMenu, GetMenuItemCount(pMainWnd->hTrayMenu), TRUE, &mii);

    // Add menu item: stop
    mii.wID        = SYSMENU_ITEM_ID_STOP;
    mii.dwTypeData = (LPTSTR) sItemStop;
    mii.cch        = lstrlen(sItemStop);

    InsertMenuItem(pMainWnd->hSysMenu,  GetMenuItemCount(pMainWnd->hSysMenu),  TRUE, &mii);
    InsertMenuItem(pMainWnd->hTrayMenu, GetMenuItemCount(pMainWnd->hTrayMenu), TRUE, &mii);

    // Add another one separator to highlight "quit" command
    InsertMenuItem(pMainWnd->hSysMenu,  GetMenuItemCount(pMainWnd->hSysMenu),  TRUE, &separator);
    InsertMenuItem(pMainWnd->hTrayMenu, GetMenuItemCount(pMainWnd->hTrayMenu), TRUE, &separator);

    // Add menu item: quit
    mii.wID        = SYSMENU_ITEM_ID_QUIT;
    mii.dwTypeData = (LPTSTR) sItemQuit;
    mii.cch        = lstrlen(sItemQuit);

    InsertMenuItem(pMainWnd->hSysMenu,  GetMenuItemCount(pMainWnd->hSysMenu),  TRUE, &mii);
    InsertMenuItem(pMainWnd->hTrayMenu, GetMenuItemCount(pMainWnd->hTrayMenu), TRUE, &mii);

    // Update menu
    DrawMenuBar(pMainWnd->hWnd);

    return EXIT_SUCCESS;
}

int MW_SystemMenu_Delete(MAIN_WINDOW* pMainWnd)
{
    if (! pMainWnd)
        return EXIT_FAILURE;

    // Destroy popup menu
    if (pMainWnd->hTrayMenu)
        DestroyMenu(pMainWnd->hTrayMenu);

    return EXIT_SUCCESS;
}

int MW_SystemMenu_Show(MAIN_WINDOW* pMainWnd)
{
    POINT ptCursor;
    GetCursorPos(&ptCursor);

    if ((! pMainWnd) || (! pMainWnd->hWnd) || (! pMainWnd->hTrayMenu))
        return EXIT_FAILURE;

    // It is needed to set window to the foreground
    // or the menu won't disappear when it should
    SetForegroundWindow(pMainWnd->hWnd);

    // Show popup menu
    TrackPopupMenu(pMainWnd->hTrayMenu,
                   TPM_BOTTOMALIGN,
                   ptCursor.x,
                   ptCursor.y,
                   0,
                   pMainWnd->hWnd,
                   NULL);

    return EXIT_SUCCESS;
}
