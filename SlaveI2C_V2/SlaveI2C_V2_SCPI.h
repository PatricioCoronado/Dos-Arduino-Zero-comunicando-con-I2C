 tipoNivel NivelDos[] = //Array de estructuras tipo Nivel
{
	//SCPI_COMANDO(COMANDO1,C1,fs1) //Comando que ejecuta la función fs1()
	//SCPI_COMANDO(COMANDO2,C2,fs2) //Comando que ejecuta la función fs2()
	//Comandos que ejecutan funciones definidas en la librería Segainvex_SCPI
	SCPI_COMANDO(ERROR,ERR,fs243)// Envía el ultimo error
	SCPI_COMANDO(*IDN,*IDN,fs240)// Identifica el instrumento
	SCPI_COMANDO(*OPC,*OPC,fs248)// Devuelve un 1 al PC
	SCPI_COMANDO(*CLS,*CLS,fs255)// Borra la pila de errores
	// TO DO Añadir aquí comandos o submenús
};
SCPI_NIVEL_RAIZ// Macro que hace la declaración obligatoria del nivel Raiz
tipoCodigoError CodigoError=
{
  // Errores del sistema SCPI 0...6
  " ",						// ERROR N. 0
  "1 Caracter no valido",                // ERROR N. 1
  "2 Comando desconocido",               // ERROR N. 2
  "3 Cadena demasiado larga",            // ERROR N. 3
  "4 Parametro inexistente",             // ERROR N. 4
  "5 Formato de parametro no valido",    // ERROR N. 5
  "6 Parametro fuera de rango",          // ERROR N. 6
  // Errores personalizados por el usuario 
  "7 El dato no esta listo",		// ERROR N. 7
};	
