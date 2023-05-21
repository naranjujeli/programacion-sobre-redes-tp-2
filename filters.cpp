#include "stdafx.h"
#include "filters.h"
#include "ppm.h"

#ifndef FILTERS_CPP
#define FILTERS_CPP

#define BLACK 0

using namespace std;

// Filtro plano como ejemplo
void plain(PPM& imagen, unsigned char c){
    for(int fila = 0; fila < imagen.alto; fila++) {
        for(int columna = 0; columna < imagen.ancho; columna++) {
            imagen.setPixel(fila, columna, Pixel(c,c,c));
        }
    }
}

void blackWhite(PPM &imagen) {
    for (int fila = 0; fila < imagen.alto; fila++) {
        for (int columna = 0; columna < imagen.ancho; columna++) {
            short int promedio = imagen.getPixel(fila, columna).sumaCumulativa()/3;
            Pixel nuevo_pixel(promedio, promedio, promedio);
            nuevo_pixel.truncar();
            imagen.setPixel(fila, columna, nuevo_pixel);
        }
    }
}

#endif