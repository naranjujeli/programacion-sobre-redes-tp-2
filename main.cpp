#include "stdafx.h"
#include "filters.h"

#define ONE_OVER_BILLION 1E-9

using namespace std;

int main(int argc , char* argv[]){
	
	// Asumimos que los filtros sin p1 se escriben primero (por lo tanto, el primer p1 es no nulo)
	// Asumimos que Zoom no se puede encadenar

	if(string(argv[1]) == "-help"){
		cout << "Uso: ./main <filtro> <nthreads> <[p1]> <img1> <custom_output> <[p2]> <img2>" << endl;
		return 0; 
	}

	// TODO Verificar que los argumentos sean correctos
	
	string filtro = string(argv[1]);
	unsigned int cantidad_threads = atoi(argv[2]);
	float primer_parametro = atof(argv[3]);
	string ruta_primera_imagen(argv[4]);
	string ruta_imagen_final = string(argv[5]);
	
	ppm primera_imagen(ruta_primera_imagen);
	
	cout << "Aplicando filtros"<< endl;

	if (filtro == "plain") {
		plain(primera_imagen, (unsigned char)primer_parametro);
	}

	cout << "Escribiendo imagen" << endl;
	imagen.write(ruta_imagen_final);	
	    
	cout << "Listo" << endl;
	return 0;
}