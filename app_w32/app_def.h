#ifndef __APP_DEF_H__
#define __APP_DEF_H__

#include "platform.h"

#define SYSMENU_ITEM_ID_SETUP 0x1010
#define SYSMENU_ITEM_ID_START 0x1020
#define SYSMENU_ITEM_ID_STOP  0x1030
#define SYSMENU_ITEM_ID_QUIT  0x1040

#define SYSMENU_TEXT_SETUP    "Configure"
#define SYSMENU_TEXT_START    "Start server"
#define SYSMENU_TEXT_STOP     "Stop server"
#define SYSMENU_TEXT_QUIT     "Stop server and exit"

#define SYS_TRAY_MSG_ID       WM_APP + 0x0001

#define STATUS_BAR_ID         0x1000
#define ADDR_LIST_ID          0x1001
#define SYS_TRAY_ICON_ID      0x1002

#define STATUS_TEXT_STOPPED   "Stopped"
#define STATUS_TEXT_STARTED   "Started"

#define WINDOW_CLASS_NAME     "cMainWindow"
#define WINDOW_TITLE_TEXT     "uDHCP server"

#define WINDOW_STATE_DEFAULT  SW_HIDE
#define WINDOW_WIDTH_DEFAULT  400
#define WINDOW_HEIGHT_DEFAULT 300

#define CUSTOM_ICC_CLASSES    ICC_WIN95_CLASSES

#endif // __APP_DEF_H__
