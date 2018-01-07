#ifndef __MW_TRAY_H__
#define __MW_TRAY_H__

#include "platform.h"
#include "mw_base.h"

int MW_SysTray_Add    (MAIN_WINDOW* pMainWnd, LPCTSTR sTip, HICON hIcon);
int MW_SysTray_Delete (MAIN_WINDOW* pMainWnd);

#endif // __MW_TRAY_H__
