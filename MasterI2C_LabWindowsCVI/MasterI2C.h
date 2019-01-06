/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2017. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PRINCIPAL                        1       /* callback function: panel_principal */
#define  PRINCIPAL_ENVIAR                 2       /* control type: command, callback function: enviar */
#define  PRINCIPAL_RECIBIR                3       /* control type: command, callback function: recibir */
#define  PRINCIPAL_STATUS                 4       /* control type: numeric, callback function: (none) */
#define  PRINCIPAL_TENSIONMEDIDA          5       /* control type: numeric, callback function: (none) */
#define  PRINCIPAL_CORRIENTEMEDIDA        6       /* control type: numeric, callback function: (none) */
#define  PRINCIPAL_COMANDO                7       /* control type: numeric, callback function: (none) */
#define  PRINCIPAL_TENSIONSP              8       /* control type: numeric, callback function: (none) */
#define  PRINCIPAL_CORRIENTESP            9       /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENU                             1
#define  MENU_COM                         2
#define  MENU_COM_CONFIGURAR              3       /* callback function: Configurar */
#define  MENU_COM_COMUNICACION            4       /* callback function: comunicacion_menu */
#define  MENU_CONFIGURACION               5
#define  MENU_INFO                        6
#define  MENU_INFO_ABOUT                  7       /* callback function: About */


     /* Callback Prototypes: */

void CVICALLBACK About(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK comunicacion_menu(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Configurar(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK enviar(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panel_principal(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK recibir(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
