#ifndef __MW_BASE_H__
#define __MW_BASE_H__

#include "platform.h"

typedef struct _MAIN_WINDOW {
    HINSTANCE      hInstance;
    HWND           hWnd;
    NOTIFYICONDATA niSysTray;
    BOOL           bTrayIcon;
    HMENU          hTrayMenu;
    HMENU          hSysMenu;
    HWND           hAddrList;
    HWND           hStatus;
    WORD           wStatusHeight;
} MAIN_WINDOW;

int MW_Window_Init (MAIN_WINDOW* pMainWnd, int nCmdShow);

#endif // __MW_BASE_H__
