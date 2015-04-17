/*****************************************************************************/
/*                                                                           */
/*                                 EEAV                                      */
/*                     Bloque de Procesado de Imagen                         */
/*                              Práctica 1                                   */
/*                                                                           */
/*****************************************************************************/
/*                             Ximo Cerdà                                    */
/*                      Valencia, Febrero de 2014                            */
/*****************************************************************************/

// Programa principal

// Includes
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <FreeImage.h>
#include <limits.h>
#include <stdlib.h>

using namespace std ; 

// variables principales
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototipos de readfile.cpp  


// Función que muestra texto por pantalla
void printHelp(string fichero) {
  std::cout << " === RASTER, Versión 1.0 === \n\n" 
    << "Programado por Ximo Cerdà\n" 
    << "Febrero de 2014\n"
	<< "Procesando...\n\n"
	<< "Realizando la lectura del fichero " << fichero << "...\n\n";
}


// Función principal
int main(int argc, char* argv[]) {
  // Sólo para ver, quitar al final
  char c;

  // Fitxer de destí
  fileName.str("default.png");

  // iMATGE
  const char *IMATGE = argv[1];

  if (argc < 2) {
    cerr << "Por favor, indica el fichero que debe renderizarse\n"; 
    exit(-1); 
  }
  
  // Preparamos el terreno: inicializamos imagen y leemos fichero
  FreeImage_Initialise();
  printHelp(argv[1]);



  //LECTURA DE LA IMATGE
  FIBITMAP *dib = FreeImage_Load(FIF_PNG, IMATGE, BMP_DEFAULT);
  FIBITMAP *src = FreeImage_ConvertTo32Bits(dib);
  //Buidem cache
  FreeImage_Unload(dib);		

  

  // Mesures de la imatge

  int width = FreeImage_GetWidth(src);
  int height = FreeImage_GetHeight(src);
  int scan_width = FreeImage_GetPitch(src);
 
  int des_gausiano[9] = {1,2,1,2,4,2,1,2,1};




  // Mapa de  bits de la imatge (buids)
  BYTE *bits = (BYTE*)malloc(height * scan_width);
  BYTE *greybits = (BYTE*)malloc(height * width);
  BYTE *outputbits = (BYTE*)malloc(width  * scan_width);
  BYTE *greybits_PG = (BYTE*)malloc(height * width );

 
  FreeImage_ConvertToRawBits(bits, src, scan_width, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
  //buidem cache
  FreeImage_Unload(src);		

// A escala de grisos
  for (int i = 0; i < height; i++) {
	  for (int j = 0; j < width; j++) {
		  float r = (float) (bits[i * scan_width + j * 4 + 0]);
		  float g = (float) (bits[i * scan_width + j * 4 + 1]);
		  float b = (float) (bits[i * scan_width + j * 4 + 2]);
		  float valor = 0.2126 * r + 0.7152 *g + 0.0722 * b;
		  greybits[i * width + j] = (BYTE) (valor);
		  greybits_PG[i * width + j] = greybits[i * width + j];              
	  }
  }	
   

//Apliquem Ponderació gausiano
  for (int i = 1; i < height; i++) {
	  for (int j = 1; j < width; j++) {
		  greybits_PG[i * width + j]=0;
		  for(int z = 0; z < 3; z++){
			for(int k=0; k < 3; k++){
				greybits_PG[i * width + j] = (greybits_PG[i * width + j]) + 0.0625*((greybits[(i+(z-1)) * width + j+(k-1)]*des_gausiano[3*z + k]));		
			}
				 
		   }	
	  }
   }

// Sortides
  for (int i = 0; i < height; i++) {
	  for (int j = 0; j < width; j++) {
		  for (int c = 0; c<3; c++) {
			outputbits[i * scan_width + j * 4 + c] = greybits_PG[i * width + j];
		  }
		outputbits[i * scan_width + j * 4 + 3] = bits[i * scan_width + j * 4 + 3];
	  }
  }


  FIBITMAP *dst = FreeImage_ConvertFromRawBits(outputbits, width,height, scan_width, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
  

  std::cout << "Proceso terminado. Guardando fichero: " << fileName.str() << "\n";
  FreeImage_Save(FIF_PNG, dst, fileName.str().c_str(), 0);

  // Liberamos memoria (limpio que es uno)
 // delete pixels;

 // Cerramos la librería y salimos
  FreeImage_DeInitialise();

  // Labores de limpieza
  free(bits);
  free(greybits);
  free(outputbits);
  free(greybits_PG);
  
  std::cout << "Guardado!";   

  return 0;
}
