// Pruebas del I2C. Código del Slave
/*El Esclavo va a ser el encargado de medir la tensión y la corriente en el filamento, a su vez va a poder realizar cambios en la fuente. Por lo tanto, va a recibir del Máster los valores de setpoint de tension y corriente y le enviará al mismo las medidas, es decir, los valores reales de tensión y de corriente.*/
/*El flujo del código es que el Máster envía el mapa de registros al Slave en cada loop y le pide datos al mismo, es decir, el ritmo del programa lo va a llevar el máster, que es el que interrumpe al Slave.*/
#include <Wire.h> //Librería para la comunicación I2C
#include <segainvex_scpi_SerialUSB.h> // Librería para la comunicación serie
#include "SlaveI2C_V2_SCPI.h"
#define  SLAVE_ADDRESS           0x29	//Dirección del Slave, cualquier número entre 0x01 y 0x7F
#define NUMERO_DE_BYTES_ENVIADOS	6
//
union DatosI2C{
	uint16_t Valores[3];
	byte  Tabla[6];     // Array de bytes de tamaño igual al tamaño de la primera variable: uint16_t = 2 bytes * 2 arrays
}DatosMaster,DatosSlave; // Se crean 2 estructuras Union, cada una es independiente de la otra, una para mandar datos del Máster al esclavo (DatosMaster) y otra para mandar datos del Esclavo al Máster (DatosSlave)
void Medida(); //En esta función se van a meter las medidas en sus variables.
//Creamos las variables donde vamos a ir guardando los valores:
uint16_t TensionSetPoint=0;
uint16_t CorrienteSetPoint=0;
uint16_t TensionMedida=0;
uint16_t CorrienteMedida=0;
uint16_t Comando=0;
uint16_t Status=0;
//
char CadenaDatos[128];
//
void setup() 
{
//Macro de Segainvex_SCPI que rellena una cadena con el nombre del sistema
NOMBRE_DEL_SISTEMA_64B(Slave I2C)
// Inicializa el I2C
Wire.begin(SLAVE_ADDRESS);// Se identifica en el bus I2C con una dirección
Wire.onRequest(requestEvent); // Callback solicitud del I2C
Wire.onReceive(receiveEvent); // Callback de la recepción I2C
// Abre el puerto serie
SerialUSB.begin(128000);
}
void loop()
{
// Si recibe algo por el puerto serie lo procesa con SEGAINVEX_SCPI
if (SerialUSB.available()){scpi();}
}// loop() 
//
/**************************************************************************************
		FUNCION QUE SE EJECUTA CUANDO SE RECIBEN DATOS DEL DESDE EL MASTER
***************************************************************************************/
void receiveEvent(int bytesReceived){
	//ISR (rutina de interrupción) receiveEvent(), en la cual se coloca el código para recibir las variables correspondientes del máster mediante Wire.read(), y meterlas en las variables temporales TensionSetpoint_Temp, CorrienteSetpoint_Temp y Err.
	//El máster llama a Wire.beginTransmission y escribe mediante Wire.write, entonces salta esta rutina de interrupción, es necesario activarla mediante Wire.onReceive() en el setup.
	SerialUSB.print("Bytes recibidos = ");
	SerialUSB.println(bytesReceived);
	if(bytesReceived == NUMERO_DE_BYTES_ENVIADOS)
	{//Si se recibe el número de datos esperados los lee 
		for (int i = 0; i < NUMERO_DE_BYTES_ENVIADOS; i++)
		DatosMaster.Tabla[i]=Wire.read();
		//Actualiza datos
		TensionSetPoint=DatosMaster.Valores[0];
		CorrienteSetPoint=DatosMaster.Valores[1];
		Comando=DatosMaster.Valores[2];
		// Copia la tabla recibida dela master en la del slave para tener algo que enviar
		DatosSlave.Valores[0]=DatosMaster.Valores[0];
		DatosSlave.Valores[1]=DatosMaster.Valores[1];
		DatosSlave.Valores[2]=DatosMaster.Valores[2];
		//
		sprintf(CadenaDatos,"Datos recibidos Tension= %d V Corriente= %d A Comando= %d",TensionSetPoint,CorrienteSetPoint ,Comando);
		SerialUSB.println(CadenaDatos);
	}
	else
	{// Si no recibe el número de datos esperado comunica un error
			SerialUSB.println("Error al recibir");
			
	}
	
	//
}
/**************************************************************************************
		FUNCION QUE EJECUTA EL SLAVE CUANDO EL MASTER I2C SOLICITA DATOS
***************************************************************************************/
void requestEvent(){
	//ISR (rutina de interrupción) requestEvent(), en la cual se coloca el código para enviar las variables correspondientes al máster mediante Wire.write()
	//Cuando el máster llama a  Wire.requestFrom salta esta rutina de interrupción, es necesario activarla mediante Wire.onRequest() en el setup.
	//Se van a enviar las medidas de tensión y de corriente
		//for (int i = 0; i <NUMERO_DE_BYTES_ENVIADOS; i++)
	//Wire.write(DatosSlave.Tabla[i]);
	Wire.write(DatosSlave.Tabla,6);
	
	sprintf(CadenaDatos,"Datos enviados Tension= %d V Corriente= %d A Status= %d",DatosSlave.Valores[0],DatosSlave.Valores[1],DatosSlave.Valores[2]);
	SerialUSB.println(CadenaDatos);
	
}
/*********************************** FIN *********************************************/

