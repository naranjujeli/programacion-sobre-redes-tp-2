#include "stdafx.h"
#include "filters.h"
#include "ppm.h"

#ifndef FILTERS_CPP
#define FILTERS_CPP

#define BLACK 0

using namespace std;

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
            Pixel pixel_final(promedio, promedio, promedio);
            imagen.setPixel(fila, columna, pixel_final);
        }
    }
}

void shades(PPM &imagen, unsigned char cantidad_grises) {
    for (int fila = 0; fila < imagen.alto; fila++) {
        for (int columna = 0; columna < imagen.ancho; columna++) {
            short int promedio = imagen.getPixel(fila, columna).sumaCumulativa()/3;
            short int valor_por_grado = 256/(cantidad_grises-1);
            short int valor_normalizado = (static_cast<short int>(promedio/valor_por_grado)) * valor_por_grado;
            Pixel pixel_final(valor_normalizado, valor_normalizado, valor_normalizado);
            imagen.setPixel(fila, columna, pixel_final);
        }
    }
}

void contrast(PPM &imagen, float contraste) {
    float factor_intensidad = (259.f * (contraste + 255.f)) / (255.f * (259.f - contraste));
    auto calculo_intensidad = [factor_intensidad] (unsigned char valor_color) -> short int { return static_cast<short int>(factor_intensidad*(valor_color-128)+128); };
    for (int fila = 0; fila < imagen.alto; fila++) {
        for (int columna = 0; columna < imagen.ancho; columna++) {
            Pixel pixel_final = imagen.getPixel(fila, columna);
            pixel_final.rojo = calculo_intensidad(pixel_final.rojo);
            pixel_final.verde = calculo_intensidad(pixel_final.verde);
            pixel_final.azul = calculo_intensidad(pixel_final.azul);
            imagen.setPixel(fila, columna, pixel_final.truncar());
        }
    }
}

#endif