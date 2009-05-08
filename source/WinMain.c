/*****************************************************************************\ 
 *
 * Crystal Quest 3D
 * Copyright (C) 2002-2009 Tor Arvid Lund
 *
 * This program is free software under the GPL license. See the file COPYING 
 * for more information.
 *
 * This is the implementation file for Windows-specific stuff.
 *
 * Written by Tor Arvid Lund.
 *
\*****************************************************************************/ 

#include "WinMain.h"

int WINAPI WinMain( HINSTANCE   hInstance,
                    HINSTANCE   hPrevInstance,
                    LPSTR       lpCmdLine,
                    int         nCmdShow)
{
    LPTSTR cmdLine;

    cmdLine = GetCommandLine();
    printf("Cmd line: %s", cmdLine);
    return main(0, NULL);
}
