#ifndef __MW_STATE_H__
#define __MW_STATE_H__

#include "platform.h"
#include "mw_base.h"

int  MW_StatusBar_Init        (MAIN_WINDOW* pMainWnd);
void MW_StatusBar_UpdateState (MAIN_WINDOW* pMainWnd, BOOL bStarted);

#endif // __MW_STATE_H__
