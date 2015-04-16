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

// Funciones auxiliares para leer un fichero

// readvals y readfile realizan un parsing muy básico
  
/*****************************************************************************/

// Includes  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>

using namespace std;
#include "variables.h" 
#include "readfile.h"


// Defines
#define MAXVERTS 500

// Lee valores de entrada (útil para el parsing)
bool readvals(stringstream &s, const int numvals, float* values) 
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            cout << "Fallo en lectura del valor " << i << ". No se considerará.\n"; 
            return false;
        }
    }
    return true; 
}


// Ésta es la función más importante del fichero. Lee un fichero de entrada
void readfile(const char* filename) 
{
    
	// Variables locales
	string str, cmd; 
    ifstream in;
	
	// Inicializamos algunos valores por defecto 
	fileName.str("default.png");		// Nombre del fichero de salida
	x_min = -1.0;
	x_max = 1.0;
	y_min = -1.0;
	y_max = 1.0;
	/*
	EXTERN int w, h;
	EXTERN int numPix; 
	EXTERN stringstream fileName;
	EXTERN int numobjects ;
	*/

	// Empezamos a leer el fichero de entrada
	in.open(filename); 
    if (in.is_open()) {

        // Es necesario implementar una pila de vértices  
		float vertStack[MAXVERTS][2]; 
		int numVert = 0;

        // Coge una linea 
        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Eliminados comentarios y líneas en blanco

                // Almacena el comando en cmd
				stringstream s(str);
                s >> cmd; 
				std::cout << "Leyendo comando " << cmd << "...\n";

				// Variables locales necesarias para parsear
                int i;
				string cadena;
                float values[10];	// Parámetros de entrada
                bool validinput;	// Validez de entrada

				// *******************************************************************************************
				// Procesado de los comandos
				// *******************************************************************************************

                // Comandos de colores
                if (cmd == "background") {
                    validinput = readvals(s, 3, values); // parámetros de color de fondo
                    if (validinput) {
						for (i = 0; i < 3; i++)
							background[i]= values[i]; 
                    }
                } else if (cmd == "foreground") {
                    validinput = readvals(s, 3, values); // parámetros de color de priemr plano
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            foreground[i] = values[i]; 
                        }
                    }

				// Comando global
				// Tamaño de la imagen a generar (debería ser el primer comando del fichero,
				// pero yo no lo obligo)
				} else if (cmd == "screen") {
                    validinput = readvals(s,2,values); 
                    if (validinput) { 
                        w = (int) values[0]; h = (int) values[1];
						numPix = w  * h;
                    } 

				// Comando global
				// Coordenadas lógicas del viewport
				} else if (cmd == "viewport") {
                    validinput = readvals(s,4,values); 
                    if (validinput) { 
                        x_min = values[0];
						x_max = values[1];
						y_min = values[2];
						y_max = values[3];
                    } 

				// Comando de Geometría
				// Objeto Tipo Vértice
				} else if (cmd == "vertex" ) {
                    if (numVert == MAXVERTS) { // No se permiten más vértices
                        cerr << "Alcanzado el número máximo de vertices " << numVert << ". Se ignorarán los que vienene a continuación.\n";
                    } else {
                        validinput = readvals(s, 2, values); 
                        if (validinput) {

							// Crea el nuevo objeto y lo mete en la pila de Vértices
                            vertStack[numVert][0] = values[0];
                            vertStack[numVert][1] = values[1];
							numVert++;
                        }
                    }
					
                // Comando de Geometría
				// Objeto Tipo Punto
				} else if (cmd == "point" ) {
                    if (numobjects == maxobjects) { // No se permiten más objetos
                        cerr << "Alcanzado el número máximo de objetos " << numobjects << ". Se ignorarán los que vienene a continuación.\n";
                    } else {
                        validinput = readvals(s, 2, values); 
                        if (validinput) {

							// Crea el nuevo objeto
                            object * obj = &(objects[numobjects]); 
                            obj->type = point;

							// primer punto
							obj->punto0[0] = vertStack[(int)values[0]][0]; 
							obj->punto1[1] = vertStack[(int)values[0]][1]; 

							// Grosor
							obj->grosor = values[1];

                            // Color
                            for (i = 0; i < 3; i++) {
                                (obj->color)[i] = foreground[i]; 
                            }
                        }
                        ++numobjects; 
                    }

				// Comando de Geometría
				// Objeto Tipo Línea
				} else if (cmd == "line" ) {
                    if (numobjects == maxobjects) { // No se permiten más objetos
                        cerr << "Alcanzado el número máximo de objetos " << numobjects << ". Se ignorarán los que vienene a continuación.\n";
                    } else {
                        validinput = readvals(s, 3, values); 
                        if (validinput) {

							// Crea el nuevo objeto
                            object * obj = &(objects[numobjects]); 
                            obj->type = line;

							// primer punto
							obj->punto0[0] = vertStack[(int)values[0]][0]; 
							obj->punto1[1] = vertStack[(int)values[0]][1]; 

							// segundo punto
							obj->punto0[0] = vertStack[(int)values[1]][0]; 
							obj->punto1[1] = vertStack[(int)values[1]][1]; 

							// Grosor
							obj->grosor = values[1];

                            // Color
                            for (i = 0; i < 3; i++) {
                                (obj->color)[i] = foreground[i]; 
                            }
						}
                        ++numobjects; 
                    }

				// Comando global
				// Nombre del fichero de salida
				} else if (cmd == "output") {
					s >> cadena; 
					if (s.fail()) {
						cout << "Fallo en lectura del nombre del ficero de salida. Se usará valor por defecto.\n"; 
			        } else {
						fileName.str(cadena);
					}
				}

				// El comando es desconocido
                else {
                    cerr << "Comando desconocido: " << cmd << ". No se considerará.\n"; 
                }
            }
            getline (in, str); 
        }

    } else {
        cerr << "No se pudo abrir el fichero de entrada " << filename << "\n"; 
        throw 2; 
    }
}
