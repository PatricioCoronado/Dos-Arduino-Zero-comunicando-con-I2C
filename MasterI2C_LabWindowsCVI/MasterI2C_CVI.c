/********************************************************************************************
	FICHERO:  PlantillaCVI.c                                                    
	
	Aplicaci�n: Plantilla para controlar instrumentaci�n virtual mediante comandos SCPI 
	
	Por Patricio Coronado Collado.17/07/2017.


	Para comunicar con el puerto COM se utilizan funciones y variables exportadas de 
	PuertoCom.h
*********************************************************************************************/
																								
/*********************************************************************************************
	Copyright � 2017 Patricio Coronado
	
	This file is part of PlantillaCVI 

    PlantillaCVI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PlantillaCVI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PlantillaCVI  If not, see <http://www.gnu.org/licenses/>

*********************************************************************************************/	
/*
	Esta cadena es la respuesta que tiene que dar el sistema que queremos controlar
	al recibir un comando SCPI IDN
*/
/*#define IDN_DEL_SISTEMA*/ char IdentidadDelSistema[]="Master I2C";
//------------------------------------------------------------------------------------------
// Ficheros include                                                          
//------------------------------------------------------------------------------------------
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <rs232.h>
#include <utility.h>
#include <formatio.h>
#include <string.h>
#include <analysis.h>
#include "MasterI2C.h"
#include "PuertoCOM.h"
#include "PanelesPuertoCom.h"
//------------------------------------------------------------------------------------------
// Constantes
//------------------------------------------------------------------------------------------
#define SI 1	  
#define NO 0
// Coordenadas de los paneles
#define PP_TOP  60	   // Panel Principal
#define PP_LEFT 60	
// Colecci�n de posibles valores de la variable DatoEsperado (dato esperado por el puerto COM)
#define NINGUNO 0
#define DATOS_RECIBIDOS 2
//-----------------------------------------------------------------------------------------
// Variables globales 
extern int Depuracion; // Variable para poner el programa en modo depuraci�n
char Comando[STRMEDIO]; //Cadena para poner un comando antes de copiarlo en CadenaComando[]
char SParametros[STRMEDIO];	// Cadena para poner un par�metros en los comandos
int Parametro; //Valor del par�metro
char DatosRecibidos[256]; //Cadena para recibir la firma del sistema   
int PrincipalHandle; //Handler del panel principal
int DatoEsperado;/*Se utiliza para decir al PC que datos se esperan del sistema 
ver la funci�n pcom_datos_recibido() en este m�dulo */
enum  {Test,Normal} Modo=Test; //Para poner el sistema en modo test o en modo de funcionamiento normal
//----------------------------------------------------------------------------------------
// Prototipo de funciones propias                                            
//----------------------------------------------------------------------------------------
void activa_controles_principal_para_comunicar(int short);// Dimar y desdimar controles
/****************************************************************************************
		                   FUNCION PRINCIPAL                                     
****************************************************************************************/
 int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	// Carga el panel principal como top-level
	if ((PrincipalHandle = LoadPanel (0, "MasterI2C.uir", PRINCIPAL)) < 0)
		return -1;
	// Carga el panel de configuraci�n del puerto como top-level
	pcom_carga_panel_de_configuracion();
	// Abre el puerto serie
	if(!pcom_abre_puerto_serie_automaticamente())
	{
		if(ConfirmPopup ("Error de puerto COM", "No se ha podido abrir ning�n puerto COM\n\n"
			"�  Quieres intentarlo manualmente?"))
			//DisplayPanel (ConfigHandle); // Pone el panel de configuraci�n en memoria
			pcom_instala_el_panel_de_configuracion(PANEL_MODO_HIJO,PP_TOP+25,PP_LEFT+25);
			//InstallPopup (ConfigHandle);
	}
	//Adem�s de cargar el panel de configuraci�n del puerto hay que cargar el de mensajes
	pcom_carga_panel_de_mensajes();
	// Muestra el panel principal
	DisplayPanel (PrincipalHandle);
	// Mira en el panel de comunicaci�n si el sistema est� o no por defecto en modo depuraci�n
	Depuracion=pcom_estado_radio_button_de_depuracion();
	if(Depuracion) pcom_muestra_el_panel_de_mensajes(PP_TOP,PP_LEFT+530);// En modo depuraci�n muestra el panel de mensajes		
	// Coloca el panel principal en una posici�n determinada en la pantalla
	SetPanelAttribute (PrincipalHandle, ATTR_TOP,PP_TOP );
	SetPanelAttribute (PrincipalHandle, ATTR_LEFT,PP_LEFT);
	// Si el puerto est� abierto activa los  controles que lo utilizan
	if(pcom_test_puerto_abierto()==0) 
	{
		activa_controles_principal_para_comunicar(SI);
		pcom_activa_controles_mensajes_para_comunicar(SI);		
		pcom_limpia_pila_errores(); // Limpia la pila de errores SCPI del micro
	}
	// Si el puerto no est� abierto desactiva los controles que comunican
	else 
	{
		activa_controles_principal_para_comunicar(NO);
		pcom_activa_controles_mensajes_para_comunicar(NO);
	}
		// Corre la aplicaci�n........................................................
		RunUserInterface (); 
	// Aqui hay que descargar todos lo paneles utilizados
	pcom_descarga_paneles_del_puerto_com();
	DiscardPanel (PrincipalHandle);
	return 0;
}
/****************************************************************************************
					FUNCIONES DEL PANEL PRINCIPAL
 	Se utiliza para salir de la aplicaci�n.Cierra el puerto y sale de la aplicaci�n.
****************************************************************************************/
int CVICALLBACK panel_principal (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	Rect CoordenadasPanel;
	//Rect CoordenadasPanelFijas={1,2,3,4};
	
	switch (event)
	{
		case EVENT_PANEL_MOVING:
		GetPanelEventRect (eventData2, &CoordenadasPanel/*Rect *rectangle*/);
		break;		
		case EVENT_GOT_FOCUS:// || EVENT_LOST_FOCUS:
			if(pcom_test_puerto_abierto()==0) 
			{
				activa_controles_principal_para_comunicar(SI);
				SetPanelAttribute (PrincipalHandle, ATTR_TITLE,IDNinstrumento);
			}
		// Si el puerto no est� abierto desactiva los controles que comunican
		else 
			{
				activa_controles_principal_para_comunicar(NO);
				SetPanelAttribute (PrincipalHandle, ATTR_TITLE,"Instrumento sin identificar"); 
			}
		break;
		//case EVENT_LOST_FOCUS:
		//break;
		case EVENT_CLOSE:
			pcom_cierra_puerto_serie();
			QuitUserInterface (0);
		break;
		case EVENT_RIGHT_CLICK:
			
		break;
	}
	return 0;
}
/****************************************************************************************
		FUNCION QUE CARGA EL PANEL DE CONFIGURACION

****************************************************************************************/
void CVICALLBACK Configurar (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	/* Carga el panel de configuraci�n como child del Principal*/
    //pcom_instala_el_panel_de_configuracion(PANEL_MODO_HIJO,PP_TOP+25,PP_LEFT+25);
	pcom_instala_el_panel_de_configuracion(PANEL_MODO_TOP,PP_TOP+25,PP_LEFT+25);
}
/****************************************************************************************
			FUNCION QUE CARGA EL PANEL DE MENSAJES
			Para que se vean los mensajes de entrada salida por el puerto COM
****************************************************************************************/
void CVICALLBACK comunicacion_menu (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	pcom_muestra_el_panel_de_mensajes(PP_TOP,PP_LEFT+530);
	
}
/****************************************************************************************
		FUNCION DE MENU PARA MOSTRAR INFORMACION DEL AUTOR
****************************************************************************************/
void CVICALLBACK About (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	int LongMensaje;
	MessagePopup 
	   ("   Autor: ", "         Patricio Coronado Collado\n"
		"        UAM-SEGAINVEX Electr�nica\n"
		"        patricio.coronado@uam.es\n"
		"Solo tiene autorizaci�n para utilizar este software\n"
		"su autor.\n"
		"Este software no forma parte de ning�n proyecto\n"
		"realizado por la UAM SEGAINVEX-Electr�nica.\n"
		"Su �nico fin es para test\n"
		);
}
/*****************************************************************************************

						PONER AQU� EL C�DIGO DE CADA APLICACION		

******************************************************************************************/




/****************************************************************************************
		FUNCION PARA DIMAR O DESDIMAR LOS CONTROLES QUE COMUNICAN
		Si "Accion" vale 1 desdima los controles y se pueden usar 
		Si "Acci�n" vale 0 dima los controles y no se pueden usar 
*****************************************************************************************/
void activa_controles_principal_para_comunicar( int short Accion)
{

	if(Accion)  // Desbloquea/activa los controles
	{
	//	SetCtrlAttribute (PrincipalHandle,PRINCIPAL_CONTROL, ATTR_DIMMED, 0);
		
	}
	else   // Bloquea/Desactiva los controles
	{
	//	SetCtrlAttribute (PrincipalHandle,PRINCIPAL_CONTROL, ATTR_DIMMED, 1);
				
	}
}
/****************************************************************************************
 FUNCION DEL PUERTO COM QUE SE DEFINE AQU� PARA QUE HAGAS LO QUE QUIERAS CUANDO RECIBAS
 UN DATO DE FORMA AUTOM�TICA.
 mirar en"CadenaRespuesta" que es donde est�n los datos recibidos
 
*****************************************************************************************/
void pcom_datos_recibido(void)
{
	unsigned int RDACRecibido;
	int Posicion,Muestra,MuestrasLeidas;
	double Medidas[257];
	switch(DatoEsperado)
	{	
		case DATOS_RECIBIDOS:
			
		break;	
	}
}
/****************************************************************************************
 							ENVIA DATOS AL SLAVE
*****************************************************************************************/

int CVICALLBACK enviar (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	static int Corriente,Tension,Comando;
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(CadenaComando,"%s","ESL "); //Pone el comando
			GetCtrlVal (panel, PRINCIPAL_COMANDO, &Comando); 
			GetCtrlVal (panel, PRINCIPAL_CORRIENTESP, &Corriente); 
			GetCtrlVal (panel, PRINCIPAL_TENSIONSP, &Tension); 
			Corriente=Corriente+2;Tension=Tension+2; Comando=Comando+3;
			SetCtrlVal (panel, PRINCIPAL_CORRIENTESP,Corriente); 
			SetCtrlVal (panel, PRINCIPAL_TENSIONSP,Tension);
			SetCtrlVal (panel, PRINCIPAL_COMANDO,Comando); 
		sprintf (SParametros, "%u %u %u",Tension,Corriente,Comando);
			strcat (CadenaComando,SParametros); // a�ade a CadenaComando el par�metro	
			ENVIAR_COMANDO_AL_SISTEMA(MOSTRAR) //
			Delay(0.2);	
		break;
	}
	return 0;
}
/****************************************************************************************
 							PIDE DATOS AL SLAVE
*****************************************************************************************/
int CVICALLBACK recibir (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int Status,CorrienteMedida,TensionMedida;
	switch (event)
	{
		case EVENT_COMMIT:
			//pcom_recepcion_automatica(NO);
			sprintf(CadenaComando,"%s","LSL");
			ENVIAR_COMANDO_AL_SISTEMA(MOSTRAR) 
			Delay(0.1);	
			sscanf(CadenaRespuesta,"%u %u %u",&TensionMedida,&CorrienteMedida,&Status);
			SetCtrlVal (panel, PRINCIPAL_STATUS, Status); 
			SetCtrlVal (panel, PRINCIPAL_CORRIENTEMEDIDA, CorrienteMedida); 
			SetCtrlVal (panel, PRINCIPAL_TENSIONMEDIDA, TensionMedida); 
			pcom_recepcion_automatica(SI);
		break;
	}
	return 0;
}
/*****************************************************************************************/
