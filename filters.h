#include "ppm.h"

#ifndef FILTERS_H
#define FILTERS_H

// FILTROS SINGLE-THREAD

void plain(PPM &imagen, unsigned char c);
void blackWhite(PPM& imagen);
void shades(PPM& imagen, unsigned char cantidad_grises);
void brightness(PPM& imagen, float brillo, int inicio, int fin);
void contrast(PPM& imagen, float contraste);
void merge(PPM& primera_imagen, PPM& segunda_imagen, float alfa_primera_imagen);
void boxBlur(PPM &imagen);
void edgeDetection(PPM &imagen);
void sharpen(PPM &imagen);


//void zoom(PPM &imagen, PPM &imagen_aumentada, int n);
//void frame(PPM& imagen, Pixel color, int x);

// FILTROS MULTI-THREAD
//void plainMT(PPM &imagen, unsigned char c);
//void blackWhiteMT(PPM& imagen);
//void shadesMT(PPM& imagen, unsigned char cantidad_grises);
//void brightnessMT(PPM& imagen, float brillo, int inicio, int fin);
//void contrastMT(PPM& imagen, float contraste);
//void mergeMT(PPM& primera_imagen, PPM& segunda_imagen, float alfa_primera_imagen);
//void boxBlurMT(PPM &imagen);
//void edgeDetectionMT(PPM &imagen);
//void sharpenMT(PPM &imagen);

#endif