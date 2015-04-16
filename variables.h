/*****************************************************************************/
/*                                                                           */
/*                                 EEAV                                      */
/*                     Bloque de Procesado de Imagen                         */
/*                              Pr�ctica 1                                   */
/*                                                                           */
/*****************************************************************************/
/*                             Ximo Cerd�                                    */
/*                      Valencia, Febrero de 2015                            */
/*****************************************************************************/

// Fichero de include con la definici�n de las variables globales del programa.

// Dado que todos los ficheros deben acceder a �ste, definimos EXTERN 
// como nada o extern, dependiendo de que est� incluido en el programa 
// principal o no.
#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif 

#ifdef MAINPROGRAM 

float x_min, x_max, y_min, y_max;	// Coordenadas l�gicas de la pantalla
int w = 256, h = 256;				// Ancho y alto en pixels de la imagen a generar
int numPix = 65536;					// N�mero total de pixels de la imagen a generar
stringstream fileName;				// Fichero de salida
int numobjects = 0 ;				// N�mero de objetos usado

#else 

EXTERN float x_min, x_max, y_min, y_max;
EXTERN int w, h;
EXTERN int numPix; 
EXTERN stringstream fileName;
EXTERN int numobjects ;

#endif 

// Tipos enumerados para formas
enum shape {point, line} ;

// Par�metros de color  (globales)
EXTERN float background[3] ; 
EXTERN float foreground[3] ; 

// Par�metros de los objetos
const int maxobjects = 100 ;		// N�mero m�ximo

EXTERN struct object {
  // S�lo 2 tipos permitidos: puntos y l�neas
  shape type ; 

  // V�lido para ambos
  float punto0[2] ;
  
  // S�lo para l�neas
  float punto1[2] ;
  
  // Configuraci�n
  float color[3] ;
  float grosor ;

} objects[maxobjects] ;
