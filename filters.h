#include "ppm.h"

#ifndef FILTERS_H
#define FILTERS_H

// FILTROS SINGLE-THREAD
void plain(PPM *imagen, const unsigned char &intensidad);
void blackWhite(PPM *imagen);
void shades(PPM *imagen, const unsigned char &cantidad_grises);
void brightness(PPM *imagen, const float &intensidad);
void contrast(PPM *imagen, const float &intensidad);
void merge(PPM *primera_imagen, PPM *segunda_imagen, const float &alfa_primera_imagen);
void boxBlur(PPM *imagen, const float &intensidad);
void edgeDetection(PPM *imagen);
void sharpen(PPM *imagen);

// FILTROS MULTI-THREAD
void plainMT(PPM *imagen, const unsigned char intensidad, const int cantidad_threads);
void blackWhiteMT(PPM *imagen, const int &cantidad_threads);
void shadesMT(PPM *imagen, const unsigned char &cantidad_grises, const int &cantidad_threads);
void brightnessMT(PPM *imagen, const float &intensidad, const int &cantidad_threads);
void contrastMT(PPM *imagen, const float &intensidad, const int &cantidad_threads);
void mergeMT(PPM *primera_imagen, PPM *segunda_imagen, const float &alfa_primera_imagen, const int &cantidad_threads);
void boxBlurMT(PPM *imagen, const float &intensidad, const int &cantidad_threads);
void edgeDetectionMT(PPM *imagen, const int &cantidad_threads);
void sharpenMT(PPM *imagen, const int &cantidad_threads);


#endif
