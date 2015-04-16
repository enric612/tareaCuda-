/*****************************************************************************/
/*                                                                           */
/*                                 EEAV                                      */
/*                     Bloque de Procesado de Imagen                         */
/*                              Pr�ctica 1                                   */
/*                                                                           */
/*****************************************************************************/
/*                             Ximo Cerd�                                    */
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


// Funci�n que muestra texto por pantalla
void printHelp(string fichero) {
  std::cout << " === RASTER, Versi�n 1.0 === \n\n" 
    << "Programado por Ximo Cerd�\n" 
    << "Febrero de 2014\n"
	<< "Procesando...\n\n"
	<< "Realizando la lectura del fichero " << fichero << "...\n\n";
}


// Funci�n principal
int main(int argc, char* argv[]) {
  // S�lo para ver, quitar al final
  char c;

  // Fitxer de dest�
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
 // readfile(argv[1]) ; ***P1*** No es necesari per aquesta practica



  // -----------------------------------------------------------------
  //    P1 Comentem tot 
  // -----------------------------------------------------------------


  // Creamos rejilla de pixels, seg�n las dimensiones globales
  //unsigned char *pixels = new unsigned char[3 * numPix];	
  //// Pero trabajaremos en flotante
  //float color[3];

  //// Primero inicializamos al color de fondo
  //for (int i = 0; i < h; i++) {
	 // for (int j = 0; j < w; j++) {

		//  // Inicilizo los pixels a un valor conocido (negro)
		//  for (int c = 0; c < 3; c++)
		//	color[c] = background[c];

		//  // Se escribe en memoria de video
		//  pixels[i * w * 3 + j * 3 + 0] = (unsigned char)(255.0 * color[0]);
		//  pixels[i * w * 3 + j * 3 + 1] = (unsigned char)(255.0 * color[1]);
		//  pixels[i * w * 3 + j * 3 + 2] = (unsigned char)(255.0 * color[2]);		
	 // }
  //}

  //// Ahora recorremos el vector de objetos
  //for (int o = 0; o < numobjects; o++) {
	 // if (objects[o].type == point) {

		//  // Coordenadas de pantalla
		//  int x_pant = (int) (w * (objects[o].punto0[0]- x_min) / ( x_max - x_min)) ;
		//  int y_pant = (int) (h * (objects[o].punto0[1]- y_min) / ( y_max - y_min)) ;
		//  
		//  // Se escribe en memoria de video. Pero antes comprobamos que est� dentro del margen de representaci�n

		//  if ((x_pant >= 0) && (y_pant >=0) && (x_pant < w) && (y_pant < h) ) {
		//		pixels[y_pant * w * 3 + x_pant * 3 + 0] = (unsigned char)(255.0 * objects[o].color[0]);
		//		pixels[y_pant * w * 3 + x_pant * 3 + 1] = (unsigned char)(255.0 * objects[o].color[1]);
		//		pixels[y_pant * w * 3 + x_pant * 3 + 2] = (unsigned char)(255.0 * objects[o].color[2]);		
		//  }
	 // }
  //}

  //// Y ya hemos terminado!!

  // -----------------------------------------------------------------
  // F�   P1 Comentem tot 
  // -----------------------------------------------------------------

  // -----------------------------------------------------------------
  // Comencem amb la P2 
  // -----------------------------------------------------------------



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
   

//Apliquem Ponderaci� gausiano
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

 // Cerramos la librer�a y salimos
  FreeImage_DeInitialise();

  // Labores de limpieza
  free(bits);
  free(greybits);
  free(outputbits);
  free(greybits_PG);
  
  std::cout << "Guardado!";   

  return 0;
}
