#include "stdafx.h"
#include "filters.h"

#ifndef FILTERS_CPP
#define FILTERS_CPP

#define BLACK 0

using namespace std;

// Filtro plano como ejemplo
void plain(ppm& img, unsigned char c){
	for(int i = 0; i < img.height; i++) {
		for(int j = 0; j < img.width; j++) {
			img.setPixel(i, j, pixel(c,c,c));
		}
	}
}

// COMPLETAR :)

#endif