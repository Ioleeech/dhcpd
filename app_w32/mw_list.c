#include "app_def.h"
#include "mw_list.h"

static int _MW_AddressList_AddLine(MAIN_WINDOW* pMainWnd, UINT uLine, LPTSTR pName, LPTSTR pMAC, LPTSTR pIP)
{
    LVITEM item = { 0 };

    item.mask  = LVIF_TEXT;
    item.iItem = uLine;

    if (! pMainWnd->hAddrList)
        return EXIT_FAILURE;

    item.iSubItem = 0;
    item.pszText  = pName;

    SendMessage(pMainWnd->hAddrList, LVM_INSERTITEM, 0, (LPARAM) &item);

    item.iSubItem = 1;
    item.pszText  = pMAC;

    SendMessage(pMainWnd->hAddrList, LVM_SETITEM, 0, (LPARAM) &item);

    item.iSubItem = 2;
    item.pszText  = pIP;

    SendMessage(pMainWnd->hAddrList, LVM_SETITEM, 0, (LPARAM) &item);

    return EXIT_SUCCESS;
}

int MW_AddressList_Init(MAIN_WINDOW* pMainWnd)
{
    DWORD dwStyle = WS_CHILD
                  | WS_VISIBLE
                  | LVS_REPORT
                  | LVS_SINGLESEL;
    DWORD dwExStyle = WS_EX_CLIENTEDGE;

    if (! pMainWnd->hWnd)
        return EXIT_FAILURE;

    // Address list will cover all window area
    RECT rc = { 0 };
    GetClientRect(pMainWnd->hWnd, &rc);

    rc.bottom -= pMainWnd->wStatusHeight;

    // Create instance of list box as a child window
    pMainWnd->hAddrList = CreateWindowEx(dwExStyle,            // DWORD     dwExStyle
                                         WC_LISTVIEW,          // LPCSTR    lpClassName
                                         NULL,                 // LPCSTR    lpWindowName
                                         dwStyle,              // DWORD     dwStyle
                                         0,                    // int       X
                                         0,                    // int       Y
                                         rc.right,             // int       nWidth
                                         rc.bottom,            // int       nHeight
                                         pMainWnd->hWnd,       // HWND      hWndParent
                                         (HMENU) ADDR_LIST_ID, // HMENU     hMenu
                                         pMainWnd->hInstance,  // HINSTANCE hInstance
                                         NULL);                // LPVOID    lpParam
    if (! pMainWnd->hAddrList)
        return EXIT_FAILURE;

    ListView_SetExtendedListViewStyle(pMainWnd->hAddrList, LVS_EX_FULLROWSELECT);

    // Address list consists from 3 columns: client name, MAC, IP
    LVCOLUMN column = { 0 };

    column.mask = LVCF_FMT
                | LVCF_WIDTH
                | LVCF_TEXT
                | LVCF_SUBITEM;
    column.fmt  = LVCFMT_LEFT;        // Left-aligned text column
    column.cx   = (rc.right - 4) / 3; // Width of column

    // 1st column
    column.iSubItem = 0;
    column.pszText  = TEXT("Name");

    SendMessage(pMainWnd->hAddrList, LVM_INSERTCOLUMN, 0, (LPARAM) &column);

    // 2nd column
    column.iSubItem = 1;
    column.pszText  = TEXT("MAC");

    SendMessage(pMainWnd->hAddrList, LVM_INSERTCOLUMN, 1, (LPARAM) &column);

    // 3rd column
    column.iSubItem = 2;
    column.pszText  = TEXT("IP");

    SendMessage(pMainWnd->hAddrList, LVM_INSERTCOLUMN, 2, (LPARAM) &column);

    // TODO
    _MW_AddressList_AddLine(pMainWnd, 0, TEXT("Host_Name_1"), TEXT("01:02:03:04:05:06"), TEXT("255.255.255.201"));
    _MW_AddressList_AddLine(pMainWnd, 1, TEXT("Host_Name_2"), TEXT("00:11:22:33:44:55"), TEXT("255.255.255.202"));
    _MW_AddressList_AddLine(pMainWnd, 2, TEXT("Host_Name_3"), TEXT("66:77:88:99:AA:BB"), TEXT("255.255.255.203"));
    _MW_AddressList_AddLine(pMainWnd, 3, TEXT("Host_Name_4"), TEXT("01:23:45:67:89:AB"), TEXT("255.255.255.204"));

    return EXIT_SUCCESS;
}
