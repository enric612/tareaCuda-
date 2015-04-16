/*****************************************************************************/
/*                                                                           */
/*                                 EEAV                                      */
/*                     Bloque de Procesado de Imagen                         */
/*                              Práctica 1                                   */
/*                                                                           */
/*****************************************************************************/
/*                             Ximo Cerdà                                    */
/*                      Valencia, Febrero de 2015                            */
/*****************************************************************************/

// Fichero de include con la definición de las variables globales del programa.

// Dado que todos los ficheros deben acceder a éste, definimos EXTERN 
// como nada o extern, dependiendo de que esté incluido en el programa 
// principal o no.
#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif 

#ifdef MAINPROGRAM 

float x_min, x_max, y_min, y_max;	// Coordenadas lógicas de la pantalla
int w = 256, h = 256;				// Ancho y alto en pixels de la imagen a generar
int numPix = 65536;					// Número total de pixels de la imagen a generar
stringstream fileName;				// Fichero de salida
int numobjects = 0 ;				// Número de objetos usado

#else 

EXTERN float x_min, x_max, y_min, y_max;
EXTERN int w, h;
EXTERN int numPix; 
EXTERN stringstream fileName;
EXTERN int numobjects ;

#endif 

// Tipos enumerados para formas
enum shape {point, line} ;

// Parámetros de color  (globales)
EXTERN float background[3] ; 
EXTERN float foreground[3] ; 

// Parámetros de los objetos
const int maxobjects = 100 ;		// Número máximo

EXTERN struct object {
  // Sólo 2 tipos permitidos: puntos y líneas
  shape type ; 

  // Válido para ambos
  float punto0[2] ;
  
  // Sólo para líneas
  float punto1[2] ;
  
  // Configuración
  float color[3] ;
  float grosor ;

} objects[maxobjects] ;
