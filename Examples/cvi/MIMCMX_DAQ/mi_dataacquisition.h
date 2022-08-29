/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2003. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                           1
#define  PANEL_INITBUTTON                2       /* callback function: vInitBoard */
#define  PANEL_STARTBUTTON               3       /* callback function: vAcquireData */
#define  PANEL_QUITBUTTON                4       /* callback function: vQuit */
#define  PANEL_WAVEFORM                  5


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK vAcquireData(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK vInitBoard(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK vQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
