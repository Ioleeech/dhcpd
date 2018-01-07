#include "app_def.h"
#include "mw_base.h"

static void _LoadCommonControls(void)
{
    // comctl32.dll v.5.x:
    // Only Windows 95 classes (ICC_WIN95_CLASSES) can be registered
    // through InitCommonControls. Programs which require additional
    // common control classes must use the InitCommonControlsEx function.
    //
    // comctl32.dll v.6.0 and later:
    // InitCommonControls does nothing. Applications must explicitly
    // register all common controls through InitCommonControlsEx.

#ifdef CUSTOM_ICC_CLASSES
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC  = CUSTOM_ICC_CLASSES;

    InitCommonControlsEx(&icc);
#else
    InitCommonControls();
#endif
}

static int _MainRoutine(MAIN_WINDOW* pMainWnd)
{
    // Main message loop
    for ( ; ; )
    {
        // Waiting for any action
        MSG     msg = { 0 };
        WINBOOL res = GetMessage(&msg, NULL, 0, 0);

        // Is it needed to exit?
        if (res <= 0)
            break;

        // React on user activity
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return EXIT_SUCCESS;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int ret = EXIT_SUCCESS;

    // Main window class
    MAIN_WINDOW stMainWnd = { 0 };
    stMainWnd.hInstance = hInstance;

    //Register common controls (load comctl32.dll)
    _LoadCommonControls();

    // Perform application initialization
    if (EXIT_SUCCESS == ret)
        ret = MW_Window_Init(&stMainWnd, WINDOW_STATE_DEFAULT); // nCmdShow

    // Main routine
    if (EXIT_SUCCESS == ret)
        ret = _MainRoutine(&stMainWnd);

    return ret;
}
