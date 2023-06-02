#include "stdafx.h"
#include "filters.h"

#define ONE_OVER_BILLION 1E-9

using namespace std;

void aplicarFiltro(PPM &imagen, void (*filtro) ());
void aplicarFiltroMT();

void validar_entrada(int argc , char* argv[],){
    if (argc <= 1){
        cout << "La entrada está vacía" << endl;
        return 0;
    }   
}
void validar_filtros(int argc , char* argv[],){
    if (filtro == "plain") {
        plain(primera_imagen, (unsigned char)primer_parametro);
    } else if (filtro == "blackwhite") {
        blackWhite(primera_imagen);
    } else if (filtro == "shades") {
        // TODO Verificar que el primer parámetro no es menor a 2
        shades(primera_imagen, (unsigned char)primer_parametro);
    } else if (filtro == "brightness") {
        // TODO Averiguar qué significa inicio y fin
        brightness(primera_imagen, primer_parametro, 0, 0);
    } else if (filtro == "contrast") {
        // TODO Limitar el contraste
        contrast(primera_imagen, primer_parametro);
    } else if (filtro == "merge") {
        // TODO Limitar porcentaje
        // TODO Verificar igual tamaño
        PPM segunda_imagen(argv[7]);
        merge(primera_imagen, segunda_imagen, primer_parametro);
    } else if (filtro == "boxblur") {
        boxBlur(primera_imagen);
    } else if (filtro == "edgedetection") {
        blackWhite(primera_imagen);
        for (int i = 0; i < 3; i++) {
            boxBlur(primera_imagen);
        }
        edgeDetection(primera_imagen);
    } else if (filtro == "sharpen") {
        sharpen(primera_imagen);
    } else {
        cout << "El filtro no existe" << endl;
        return 0;
    }
}

void parametro_ayuda(int argc , char* argv[]){
    if (string(argv[1]) == "-help") {
        cout << "Uso: ./main <filtro> <nthreads> <[p1]> <img1> <custom_output> <[p2]> <img2>" << endl;
        return 0; 
    } 
}



int main(int argc , char* argv[]) {
    
    // Asumimos que los filtros sin p1 se escriben primero (por lo tanto, el primer p1 es no nulo)
    // Asumimos que Zoom no se puede encadenar
    validar_entrada(argc, argv);
    parametro_ayuda(argc, argv);
    validar_filtros(argc, argv);

    // TODO Verificar que los argumentos sean correctos
    
    
    unsigned int cantidad_threads = atoi(argv[2]);
    
    float primer_parametro = atof(argv[3]);

    string ruta_primera_imagen = string(argv[4]);
    PPM primera_imagen(ruta_primera_imagen);
    string ruta_imagen_final = string(argv[5]);    
    
    cout << "Aplicando filtros"<< endl;

    if (nthreads > 1) {

    } else {
        if (filtro == "plain") {
            plain(primera_imagen, (unsigned char)primer_parametro);
        } else if (filtro == "blackwhite") {
            blackWhite(primera_imagen);
        } else if (filtro == "shades") {
            // TODO Verificar que el primer parámetro no es menor a 2
            shades(primera_imagen, (unsigned char)primer_parametro);
        } else if (filtro == "brightness") {
            // TODO Averiguar qué significa inicio y fin
            brightness(primera_imagen, primer_parametro, 0, 0);
        } else if (filtro == "contrast") {
            // TODO Limitar el contraste
            contrast(primera_imagen, primer_parametro);
        } else if (filtro == "merge") {
            // TODO Limitar porcentaje
            // TODO Verificar igual tamaño
            PPM segunda_imagen(argv[7]);
            merge(primera_imagen, segunda_imagen, primer_parametro);
        } else if (filtro == "boxblur") {
            boxBlur(primera_imagen);
        } else if (filtro == "edgedetection") {
            blackWhite(primera_imagen);
            for (int i = 0; i < 3; i++) {
                boxBlur(primera_imagen);
            }
            edgeDetection(primera_imagen);
        } else if (filtro == "sharpen") {
            sharpen(primera_imagen);
        }
    }

    cout << "Escribiendo imagen" << endl;
    primera_imagen.escribir_imagen(ruta_imagen_final);	
        
    cout << "Listo" << endl;
    return 0;
}