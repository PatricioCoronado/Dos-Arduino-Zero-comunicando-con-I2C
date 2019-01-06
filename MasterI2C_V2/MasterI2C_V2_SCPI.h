 tipoNivel NivelDos[] = //Array de estructuras tipo Nivel
{
	SCPI_COMANDO(ESCRIBESLAVE,ESL,fs1) //Manda datos al slave datos al slave
	SCPI_COMANDO(LEESLAVE,LSL,fs2) //Pide datos al slave y los recibe
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
