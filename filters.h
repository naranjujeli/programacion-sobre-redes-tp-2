#include "ppm.h"

#ifndef FILTERS_H
#define FILTERS_H

// FILTROS SINGLE-THREAD

void blackWhite(PPM& imagen);
void contrast(PPM& imagen, float contraste);
void brightness(PPM& imagen, float brillo, int inicio, int fin);
void shades(PPM& imagen, unsigned char sombras);
void merge(PPM& primera_imagen, PPM& segunda_imagen, float alfa);
void frame(PPM& imagen, Pixel color, int x);
void boxBlur(PPM &imagen);
void zoom(PPM &imagen, PPM &imagen_aumentada, int n);
void edgeDetection(PPM &imagen, PPM &imagen_target);
void plain(PPM &imagen, unsigned char c);

// FILTROS MULTI-THREAD

// Completar :)

#endif