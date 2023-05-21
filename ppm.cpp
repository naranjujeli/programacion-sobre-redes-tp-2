#include "stdafx.h"
#include "ppm.h"

#ifndef PPM_CPP
#define PPM_CPP

unsigned short int truncarValor(float valor) { 
    if (valor > 255) {
        return 255;
    } else if (valor < 0) {
        return 0;
    }
    return static_cast<unsigned short int>(valor);
}

void PPM::inicializar(unsigned int ancho, unsigned int alto) {
    this->ancho = ancho;
    this->alto = alto;
    this->valor_maximo_colores = 255;
    this->tamanio = ancho*alto;
}

PPM::PPM() {
    inicializar(0, 0);
}

PPM::PPM(unsigned int ancho, unsigned int alto){
    inicializar(ancho, alto);
    this->bitmap.resize(alto);
    for (unsigned int i = 0; i < alto; i++) {
        this->bitmap[i].resize(ancho);
    }
}

PPM::PPM(const std::string &ruta) {
    inicializar(0,0);
    leer_imagen(ruta);
}

void PPM::leer_imagen(const std::string &ruta) {
    std::ifstream archivo_entrada(ruta.c_str(), ios::in | ios::binary);
    if (archivo_entrada.is_open()) {
        std::string linea;
        
        // ¿El archivo empieza con "P6"? Es .ppm
        getline(archivo_entrada, linea);
        if (linea != "P6") {
            cout << "Error. Unrecognized file format." <<  endl;
            return;
        }

        // Ignorar los comentarios
        getline(archivo_entrada, linea);
        while (linea[0] == '#') {
             getline(archivo_entrada, linea);
        }
        
        // Línea con ancho y alto de la imagen
        stringstream dimensiones_imagen(linea);
        try {
            dimensiones_imagen >> this->ancho;
            dimensiones_imagen >> this->alto;
            this->cantidad_filas = this->alto;
            this->cantidad_columnas = this->ancho;
        } catch (std::exception &e) {
             cout << "Header file format error. " << e.what() <<  endl;
            return;
        }

        // Obtener el valor máximo de cada pixel
        getline(archivo_entrada, linea);
        stringstream valor_maximo(linea);
        try {
            valor_maximo >> this->valor_maximo_colores;
        } catch (std::exception &e) {
            cout << "Header file format error. " << e.what() <<  endl;
            return;
        }

        this->tamanio = this->ancho*this->alto;

        // Reasignar el tamaño de la matriz
        this->bitmap.resize(this->alto);
        for(unsigned int i = 0; i < this->alto; i++) {
            this->bitmap[i].resize(this->ancho);
        }
        
        // Escribir la informacion en la matriz
        char aux;
        for (unsigned int i = 0; i < this->tamanio; ++i) {
            archivo_entrada.read(&aux, 1);
            this->bitmap[i/this->ancho][i%this->ancho].rojo = (unsigned char)aux;
            archivo_entrada.read(&aux, 1);
            this->bitmap[i/this->ancho][i%this->ancho].verde = (unsigned char)aux;
            archivo_entrada.read(&aux, 1);
            this->bitmap[i/this->ancho][i%this->ancho].azul = (unsigned char)aux;
        }

    } else {
         cout << "Error. Unable to open " << ruta <<  endl;
    }
    archivo_entrada.close();
}

void PPM::escribir_imagen(const  string &ruta) {
    ofstream archivo_salida(ruta.c_str(), ios::out | ios::binary);
    if (archivo_salida.is_open()) {

        archivo_salida << "P6" << std::endl;
        archivo_salida << this->ancho;
        archivo_salida << " ";
        archivo_salida << this->alto << std::endl;
        archivo_salida << this->valor_maximo_colores << std::endl;

        char aux;
        for (unsigned int i = 0; i < alto; ++i) {
            for(unsigned int j = 0; j < ancho; j++){
                aux = (char) this->bitmap[i][j].rojo;
                archivo_salida.write(&aux, 1);
                aux = (char) this->bitmap[i][j].verde;
                archivo_salida.write(&aux, 1);
                aux = (char) this->bitmap[i][j].azul;
                archivo_salida.write(&aux, 1);
            }
        }

    } else {
        std::cout << "Error. Unable to open " << ruta << std::endl;
    }
    archivo_salida.close();
}

#endif