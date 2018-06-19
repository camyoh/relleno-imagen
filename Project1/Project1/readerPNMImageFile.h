/*
Rutinas para la lectura de imagenes en formato pnm (P3)
Más información de la estructura del archivo en
https://en.wikipedia.org/wiki/Netpbm_format

Wilson Javier Sarmiento, 2015
Universidad Militar Nueva Granada
Bogotá D.C. Colombia
*/

#ifndef __readerPNMImageFile__
#define __readerPNMImageFile__

//--------------------------------------------------------------------------------
/*
	Función de verificación de un archivo PNM.
	Permite verificar si un archivo determinado
	es un archivo de imagen digital PNM.
	La verificación consiste en leer y comparar los dos
	primeros caracteres del archivo con la palabra clave P3.
	Si el archivo es correcto, se lee el alto y el acho de
	la imagen y se retornan sus valores en los parámetros
	H y W. 

	La función retorna true sí el archivo es correcto,
	y false	en otro caso.

	Parámetros:
	fileName, nombre del archivo.
	H, variable donde se recibe el alto de la imagen.
	H, variable donde se recibe el ancho de la imagen.
*/
bool checkPNMFile(char* fileName, int* H, int * W);
//--------------------------------------------------------------------------------
/*
	Función de lectura de un archivo PNM.

	La función retorna true sí el archivo es correcto,
	y false	en otro caso.

	Parámetros:
	fileName, nombre del archivo.
	Image, matriz de enteros de [alto][ancho][3] donde 
		se va almacenar la imagen. 
*/
bool readPNMFile(char* fileName, int*** Image);
//--------------------------------------------------------------------------------

#endif // __readerPNMImageFile__

