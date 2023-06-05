#include "ppm.h"

#ifndef FILTERS_H
#define FILTERS_H

// FILTROS SINGLE-THREAD

void plain(PPM &imagen, const unsigned char &intensidad);
void blackWhite(PPM &imagen);
void shades(PPM &imagen, const unsigned char &cantidad_grises);
void brightness(PPM &imagen, const float &intensidad);
void contrast(PPM &imagen, const float &intensidad);
void merge(PPM &primera_imagen, PPM &segunda_imagen, const float &alfa_primera_imagen);
void boxBlur(PPM &imagen, const float &intensidad);
void edgeDetection(PPM &imagen);
void sharpen(PPM &imagen);

// FILTROS MULTI-THREAD

#endif
