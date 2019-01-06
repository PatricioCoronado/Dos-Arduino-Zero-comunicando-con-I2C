// Pruebas del I2C. Código del Master
#include <Wire.h>
#include <segainvex_scpi_SerialUSB.h>
#include "MasterI2C_V2_SCPI.h"
#define  SLAVE_ADDRESS           0x29	//Dirección del Slave, cualquier número entre 0x01 y 0x7F
#define NUMERO_DE_BYTES_ENVIADOS	6
union DatosI2C{
	uint16_t Valores[3];
	byte  Tabla[6];     // Array de bytes de tamaño igual al tamaño de la primera variable: uint16_t = 2 bytes * 2 arrays
}DatosMaster,DatosSlave; // Se crean 2 estructuras Union, cada una es independiente de la otra, una para mandar datos del Máster al esclavo (DatosMaster) y otra para mandar datos del Esclavo al Máster (DatosSlave)
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
// Inicializa el I2C
Wire.begin();
// Abre el puerto serie
SerialUSB.begin(128000); 
}
void loop()
{
  // Si recibe algo por el puerto serie lo procesa con SEGAINVEX_SCPI 
  if (SerialUSB.available()){scpi();}
}// loop() 
/**************************************************************************************
			FUNCION PARA QUE EL MASTER PIDA Y RECIBA DATOS DEL SLAVE
***************************************************************************************/
void fs2(void)//SCPI_COMANDO(LEESLAVE,LSL,fs2) //Pide datos al slave
{
	//En esta función se le piden al esclavo las medidas de tensión y corriente.
	//Ahora mediante la función requestFrom() se piden las medidas y se guardan en el mapa de registros mediante Wire.read():
	SerialUSB.println("solicita datos al slave");
	Wire.requestFrom(SLAVE_ADDRESS, NUMERO_DE_BYTES_ENVIADOS);
	for (int i = 0; i < NUMERO_DE_BYTES_ENVIADOS; i++)
	DatosSlave.Tabla[i]=Wire.read();
	//
	TensionMedida=DatosSlave.Valores[0];
	CorrienteMedida=DatosSlave.Valores[1];
	Status=DatosSlave.Valores[2];
	//Envía datos al PC
	sprintf(CadenaDatos,"%u %u %u",DatosSlave.Valores[0],DatosSlave.Valores[1],DatosSlave.Valores[2]);
	SerialUSB.println(CadenaDatos);
}
/**************************************************************************************
			FUNCION PARA QUE EL MASTER ESCRIBA AL SLAVE
***************************************************************************************/
void fs1(void)//SCPI_COMANDO(ESCRIBESLAVE,ESL,fs6) //Manda datos al slave
{
	int Var0,Var1,Var2; //variables intermedias
	unsigned int ParametrosALeer=3; // El PC envía 3 parámetros
	unsigned int ParametrosLeidos; // número de parámetros leidos del PC
	//Lee los datos que envía el PC
	// Si el primer carácter de FinComado es 'espacio' lee los parámetros
	 if(FinComando[0]==' ')
	 {// Lee los parámetros de la cadena de comando
		 ParametrosLeidos = sscanf(FinComando,"%u %u %u",&Var0,&Var1,&Var2);
		 //ParametrosLeidos = sscanf(FinComando,"%u %u",&Var0,&Var1);
		  //Si no leen 3 parámetro¡Error!:Formato de parámetro no válido
		 if(ParametrosLeidos != ParametrosALeer){errorscpi(5);return;}
	 }
	 // Si el comando no empieza por 'espacio'¡Error!Parámetros inexistentes.
	 else {errorscpi(4);return;} 
		 
	 //En esta función se le envían al esclavo los setpoints de tensión y corriente.
	 //Para ello se comienza la transmisión mediante Wire.beginTransmission(SLAVE_ADDRESS) y luego se envían los datos mediante Wire.write():
	 DatosMaster.Valores[0]=Var0;
	 DatosMaster.Valores[1]=Var1;// Copia los datos en la tabla...
	 DatosMaster.Valores[2]=Var2;
	  Wire.beginTransmission(SLAVE_ADDRESS);
	 for (int i = 0; i <NUMERO_DE_BYTES_ENVIADOS; i++)
	 Wire.write(DatosMaster.Tabla[i]);
	 Wire.endTransmission();//hay que terminar la transmisión
	 SerialUSB.println("escribe estos datos al slave");
	 sprintf(CadenaDatos,"%u %u %u",DatosMaster.Valores[0],DatosMaster.Valores[1],DatosMaster.Valores[2]);
	 SerialUSB.println(CadenaDatos);
	 
}
/*************************************FIN********************************************/
