#include "stdafx.h"
#include "filters.h"
#include "ppm.h"

#ifndef FILTERS_CPP
#define FILTERS_CPP

#define BLACK 0

using namespace std;

void plain(PPM& imagen, const unsigned char &intensidad){
    for(int fila = 0; fila < imagen.alto; fila++) {
        for(int columna = 0; columna < imagen.ancho; columna++) {
            imagen.setPixel(fila, columna, Pixel(intensidad,intensidad,intensidad));
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

void shades(PPM &imagen, const unsigned char &cantidad_grises) {
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

void brightness(PPM &imagen, const float &intensidad) {
    for (int fila = 0; fila < imagen.alto; fila++) {
        for (int columna = 0; columna < imagen.ancho; columna++) {
            Pixel pixel_final = imagen.getPixel(fila, columna);
            pixel_final.sumar(255.f*intensidad);
            imagen.setPixel(fila, columna, pixel_final.truncar());
        }
    }
}

void contrast(PPM &imagen, const float &intensidad) {
    float factor_intensidad = (259.f * (intensidad + 255.f)) / (255.f * (259.f - intensidad));
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

void merge(PPM &primera_imagen, PPM &segunda_imagen, const float &alfa_primera_imagen) {
    float alfa_segunda_imagen = 1.f - alfa_primera_imagen;
    for (int fila = 0; fila < primera_imagen.alto; fila++) {
        for (int columna = 0; columna < primera_imagen.ancho; columna++) {
            Pixel pixel_primera_imagen = primera_imagen.getPixel(fila, columna);
            Pixel pixel_segunda_imagen = segunda_imagen.getPixel(fila, columna);

            pixel_primera_imagen.multiplicar(alfa_primera_imagen);
            pixel_segunda_imagen.multiplicar(alfa_segunda_imagen);
            pixel_primera_imagen.sumarPixel(pixel_segunda_imagen);
            
            primera_imagen.setPixel(fila, columna, pixel_primera_imagen);
        }
    }
}

void boxBlur(PPM &imagen, const float &intensidad) {
    PPM imagen_final(imagen.ancho, imagen.alto);
    for (int fila = 1; fila < imagen.alto-1; fila++) {
        for (int columna = 1; columna < imagen.ancho-1; columna++) {
            Pixel suma;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    suma.sumarPixel(imagen.getPixel(fila+i, columna+j));
                }
            }
            suma.multiplicar(intensidad);
            imagen_final.setPixel(fila, columna, suma.truncar());
        }
    }
    imagen = imagen_final;
}

void edgeDetection(PPM &imagen) {
    short int kernel_horizontal[3][3] = {
        { 1, 2, 1 },
        { 0, 0, 0 },
        { -1, -2, -1 }
    };
    short int kernel_vertical[3][3] = {
        { 1, 0, -1 },
        { 2, 0, -2 },
        { 1, 0, -1 }
    };

    PPM imagen_final(imagen.ancho, imagen.alto);
    for (int fila = 1; fila < imagen.alto-1; fila++) {
        for (int columna = 1; columna < imagen.ancho-1; columna++) {
            // TODO crear una funcion para aplicar el kernel
            int filtro_vertical;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    filtro_vertical += imagen.getPixel(fila+i, columna+j).rojo * kernel_vertical[i+1][j+1];
                }
            }

            filtro_vertical /= 9;

            int filtro_horizontal;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    filtro_horizontal += imagen.getPixel(fila+i, columna+j).rojo * kernel_horizontal[i+1][j+1];
                }
            }

            filtro_horizontal /= 9;

            short int pitagoras = static_cast<short int>(std::sqrt(std::pow(filtro_vertical, 2) + std::pow(filtro_horizontal, 2)));

            Pixel pixel_final(pitagoras, pitagoras, pitagoras);
            imagen_final.setPixel(fila, columna, pixel_final.truncar());
        }
    }
    for (int fila = 1; fila < imagen.alto-1; fila++) {
        for (int columna = 1; columna < imagen.ancho-1; columna++) {
            imagen.setPixel(fila, columna, imagen_final.getPixel(fila, columna)); 
        }
    }
}

void sharpen(PPM &imagen) {
    std::vector<std::vector<int>> kernel = {
        { 0, -1, 0 },
        { -1, 5, -1},
        { 0, -1, 0 }
    };
    PPM imagen_final(imagen.ancho, imagen.alto);
    for (int fila = 1; fila < imagen.alto-1; fila++) {
        for (int columna = 1; columna < imagen.ancho-1; columna++) {
            Pixel suma;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    Pixel pixel_auxiliar = imagen.getPixel(fila+i, columna+j);
                    suma.sumarPixel(pixel_auxiliar.multiplicar(kernel[i+1][j+1]));
                }
            }
            imagen_final.setPixel(fila, columna, suma.truncar());
        }
    }
    for (int fila = 1; fila < imagen.alto-1; fila++) {
        for (int columna = 1; columna < imagen.ancho-1; columna++) {
            imagen.setPixel(fila, columna, imagen_final.getPixel(fila, columna)); 
        }
    }
}



#endif
