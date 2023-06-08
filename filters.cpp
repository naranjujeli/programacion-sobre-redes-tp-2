#include "stdafx.h"
#include "filters.h"
#include "ppm.h"

#ifndef FILTERS_CPP
#define FILTERS_CPP

#define BLACK 0

using namespace std;

// Single-Thread

void plain(PPM *imagen, const unsigned char &intensidad){
    for(int fila = 0; fila < imagen->alto; fila++) {
        for(int columna = 0; columna < imagen->ancho; columna++) {
            imagen->setPixel(fila, columna, Pixel(intensidad,intensidad,intensidad));
        }
    }
}

void blackWhite(PPM *imagen) {
    for (int fila = 0; fila < imagen->alto; fila++) {
        for (int columna = 0; columna < imagen->ancho; columna++) {
            short int promedio = imagen->getPixel(fila, columna).sumaCumulativa()/3;
            Pixel pixel_final(promedio, promedio, promedio);
            imagen->setPixel(fila, columna, pixel_final);
        }
    }
}

void shades(PPM *imagen, const unsigned char &cantidad_grises) {
    for (int fila = 0; fila < imagen->alto; fila++) {
        for (int columna = 0; columna < imagen->ancho; columna++) {
            short int promedio = imagen->getPixel(fila, columna).sumaCumulativa()/3;
            short int valor_por_grado = 256/(cantidad_grises-1);
            short int valor_normalizado = (static_cast<short int>(promedio/valor_por_grado)) * valor_por_grado;
            Pixel pixel_final(valor_normalizado, valor_normalizado, valor_normalizado);
            imagen->setPixel(fila, columna, pixel_final);
        }
    }
}

void brightness(PPM *imagen, const float &intensidad) {
    for (int fila = 0; fila < imagen->alto; fila++) {
        for (int columna = 0; columna < imagen->ancho; columna++) {
            Pixel pixel_final = imagen->getPixel(fila, columna);
            pixel_final.sumar(255.f * intensidad);
            imagen->setPixel(fila, columna, pixel_final.truncar());
        }
    }
}

void contrast(PPM *imagen, const float &intensidad) {
    float factor_intensidad = (259.f * (intensidad + 255.f)) / (255.f * (259.f - intensidad));
    auto calculo_intensidad = [factor_intensidad] (unsigned char valor_color) -> short int { return static_cast<short int>(factor_intensidad*(valor_color-128)+128); };
    for (int fila = 0; fila < imagen->alto; fila++) {
        for (int columna = 0; columna < imagen->ancho; columna++) {
            Pixel pixel_final = imagen->getPixel(fila, columna);
            pixel_final.rojo = calculo_intensidad(pixel_final.rojo);
            pixel_final.verde = calculo_intensidad(pixel_final.verde);
            pixel_final.azul = calculo_intensidad(pixel_final.azul);
            imagen->setPixel(fila, columna, pixel_final.truncar());
        }
    }
}

void merge(PPM *primera_imagen, PPM *segunda_imagen, const float &alfa_primera_imagen) {
    float alfa_segunda_imagen = 1.f - alfa_primera_imagen;
    for (int fila = 0; fila < primera_imagen->alto; fila++) {
        for (int columna = 0; columna < primera_imagen->ancho; columna++) {
            Pixel pixel_primera_imagen = primera_imagen->getPixel(fila, columna);
            Pixel pixel_segunda_imagen = segunda_imagen->getPixel(fila, columna);

            pixel_primera_imagen.multiplicar(alfa_primera_imagen);
            pixel_segunda_imagen.multiplicar(alfa_segunda_imagen);
            pixel_primera_imagen.sumarPixel(pixel_segunda_imagen);
            
            primera_imagen->setPixel(fila, columna, pixel_primera_imagen);
        }
    }
}

void boxBlur(PPM *imagen, const float &intensidad) {
    PPM imagen_auxiliar = PPM(imagen->ancho, imagen->alto);
    for (int fila = 1; fila < imagen->alto-1; fila++) {
        for (int columna = 1; columna < imagen->ancho-1; columna++) {
            Pixel suma;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    suma.sumarPixel(imagen->getPixel(fila+i, columna+j));
                }
            }
            Pixel promedio = suma.multiplicar(1.f/9.f);
            imagen_auxiliar.setPixel(fila, columna, promedio.truncar());
        }
    }
    for (int fila = 1; fila < imagen->alto-1; fila++) {
        for (int columna = 1; columna < imagen->ancho-1; columna++) {
            imagen->setPixel(fila, columna, imagen_auxiliar.getPixel(fila, columna));
        }
    }
}

void edgeDetection(PPM *imagen) {
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

    PPM imagen_auxiliar(imagen->ancho, imagen->alto);
    for (int fila = 1; fila < imagen->alto-1; fila++) {
        for (int columna = 1; columna < imagen->ancho-1; columna++) {
            // TODO crear una funcion para aplicar el kernel
            int filtro_vertical;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    filtro_vertical += imagen->getPixel(fila+i, columna+j).rojo * kernel_vertical[i+1][j+1];
                }
            }

            filtro_vertical /= 9;

            int filtro_horizontal;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    filtro_horizontal += imagen->getPixel(fila+i, columna+j).rojo * kernel_horizontal[i+1][j+1];
                }
            }

            filtro_horizontal /= 9;

            short int pitagoras = static_cast<short int>(std::sqrt(std::pow(filtro_vertical, 2) + std::pow(filtro_horizontal, 2)));

            Pixel pixel_final(pitagoras, pitagoras, pitagoras);
            imagen_auxiliar.setPixel(fila, columna, pixel_final.truncar());
        }
    }
    for (int fila = 1; fila < imagen->alto-1; fila++) {
        for (int columna = 1; columna < imagen->ancho-1; columna++) {
            imagen->setPixel(fila, columna, imagen_auxiliar.getPixel(fila, columna));
        }
    }
}

void sharpen(PPM *imagen) {
    std::vector<std::vector<int>> kernel = {
        { 0, -1, 0 },
        { -1, 5, -1},
        { 0, -1, 0 }
    };
    PPM imagen_auxiliar(imagen->ancho, imagen->alto);
    for (int fila = 1; fila < imagen->alto-1; fila++) {
        for (int columna = 1; columna < imagen->ancho-1; columna++) {
            Pixel suma;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    Pixel pixel_auxiliar = imagen->getPixel(fila+i, columna+j);
                    suma.sumarPixel(pixel_auxiliar.multiplicar(kernel[i+1][j+1]));
                }
            }
            imagen_auxiliar.setPixel(fila, columna, suma.truncar());
        }
    }
    for (int fila = 1; fila < imagen->alto-1; fila++) {
        for (int columna = 1; columna < imagen->ancho-1; columna++) {
            imagen->setPixel(fila, columna, imagen_auxiliar.getPixel(fila, columna));
        }
    }
}

// Multi-Thread

struct ArgumentosMT {
    PPM *primera_imagen;
    PPM *segunda_imagen;
    sem_t *semaforo;
    const float parametro;
    const int cantidad_threads;
    const int fila_inicial;
    ArgumentosMT(PPM *primera_imagen, sem_t *semaforo, const float parametro, const int cantidad_threads, const int fila_inicial, PPM *segunda_imagen=nullptr) : 
        primera_imagen(primera_imagen), semaforo(semaforo), parametro(parametro), cantidad_threads(cantidad_threads), fila_inicial(fila_inicial), segunda_imagen(segunda_imagen) {}
};

void *procedimientoPlain(void *argumento) {
    ArgumentosMT *estructura_argumentos = (ArgumentosMT *)argumento;

    sem_wait(estructura_argumentos->semaforo);
    const int ancho_imagen = estructura_argumentos->primera_imagen->ancho;
    const int alto_imagen = estructura_argumentos->primera_imagen->alto;
    sem_post(estructura_argumentos->semaforo);

    for (int fila = estructura_argumentos->fila_inicial; fila < alto_imagen; fila += estructura_argumentos->cantidad_threads) {
        for (int columna = 0; columna < ancho_imagen; columna++) {
            Pixel nuevo_pixel(estructura_argumentos->parametro, estructura_argumentos->parametro, estructura_argumentos->parametro);
            sem_wait(estructura_argumentos->semaforo);
            estructura_argumentos->primera_imagen->setPixel(fila, columna, nuevo_pixel);
            sem_post(estructura_argumentos->semaforo);
        }
    }

    return nullptr;
}

void plainMT(PPM *imagen, const unsigned char intensidad, const int cantidad_threads) {

    std::vector<pthread_t> threads(cantidad_threads);
    std::vector<ArgumentosMT *> argumentos(cantidad_threads);
    sem_t semaforo;
    sem_init(&semaforo, 0, 1);

    for (int i = 0; i < cantidad_threads; i++) {
        argumentos[i] = new ArgumentosMT(imagen, &semaforo, intensidad, cantidad_threads, i);
        pthread_create(&threads[i], NULL, procedimientoPlain, argumentos[i]);
    }
    for (int i = 0; i < cantidad_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < cantidad_threads; i++) {
        delete argumentos[i];
    }
    sem_destroy(&semaforo);
}

void *procedimientoBlackWhite(void *argumento) {
    ArgumentosMT *estructura_argumentos = (ArgumentosMT *)argumento;

    sem_wait(estructura_argumentos->semaforo);
    const int ancho_imagen = estructura_argumentos->primera_imagen->ancho;
    const int alto_imagen = estructura_argumentos->primera_imagen->alto;
    sem_post(estructura_argumentos->semaforo);

    for (int fila = estructura_argumentos->fila_inicial; fila < alto_imagen; fila += estructura_argumentos->cantidad_threads) {
        for (int columna = 0; columna < ancho_imagen; columna++) {
            sem_wait(estructura_argumentos->semaforo);
            short int promedio = estructura_argumentos->primera_imagen->getPixel(fila, columna).sumaCumulativa()/3;
            sem_post(estructura_argumentos->semaforo);
            
            Pixel pixel_final(promedio, promedio, promedio);
            
            sem_wait(estructura_argumentos->semaforo);
            estructura_argumentos->primera_imagen->setPixel(fila, columna, pixel_final);
            sem_post(estructura_argumentos->semaforo);
        }
    }

    return nullptr;
}

void blackWhiteMT(PPM *imagen, const int &cantidad_threads) {
    std::vector<pthread_t> threads(cantidad_threads);
    std::vector<ArgumentosMT *> argumentos(cantidad_threads);
    sem_t semaforo;
    sem_init(&semaforo, 0, 1);

    for (int i = 0; i < cantidad_threads; i++) {
        argumentos[i] = new ArgumentosMT(imagen, &semaforo, 0, cantidad_threads, i);
        pthread_create(&threads[i], NULL, procedimientoBlackWhite, argumentos[i]);
    }
    for (int i = 0; i < cantidad_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < cantidad_threads; i++) {
        delete argumentos[i];
    }
    sem_destroy(&semaforo);
}

void *procedimientoShades(void *argumento) {
    ArgumentosMT *estructura_argumentos = (ArgumentosMT *)argumento;

    sem_wait(estructura_argumentos->semaforo);
    const int ancho_imagen = estructura_argumentos->primera_imagen->ancho;
    const int alto_imagen = estructura_argumentos->primera_imagen->alto;
    sem_post(estructura_argumentos->semaforo);

    for (int fila = estructura_argumentos->fila_inicial; fila < alto_imagen; fila += estructura_argumentos->cantidad_threads) {
        for (int columna = 0; columna < ancho_imagen; columna++) {
            sem_wait(estructura_argumentos->semaforo);
            short int promedio = estructura_argumentos->primera_imagen->getPixel(fila, columna).sumaCumulativa()/3;
            sem_post(estructura_argumentos->semaforo);
            
            short int valor_por_grado = 256/(estructura_argumentos->parametro-1);
            short int valor_normalizado = (static_cast<short int>(promedio/valor_por_grado)) * valor_por_grado;
            Pixel pixel_final(valor_normalizado, valor_normalizado, valor_normalizado);
            Pixel nuevo_pixel(estructura_argumentos->parametro, estructura_argumentos->parametro, estructura_argumentos->parametro);
            
            sem_wait(estructura_argumentos->semaforo);
            estructura_argumentos->primera_imagen->setPixel(fila, columna, nuevo_pixel);
            sem_post(estructura_argumentos->semaforo);
        }
    }

    return nullptr;
}

void shadesMT(PPM *imagen, const unsigned char &cantidad_grises, const int &cantidad_threads) {
    std::vector<pthread_t> threads(cantidad_threads);
    std::vector<ArgumentosMT *> argumentos(cantidad_threads);
    sem_t semaforo;
    sem_init(&semaforo, 0, 1);

    for (int i = 0; i < cantidad_threads; i++) {
        argumentos[i] = new ArgumentosMT(imagen, &semaforo, cantidad_grises, cantidad_threads, i);
        pthread_create(&threads[i], NULL, procedimientoShades, argumentos[i]);
    }
    for (int i = 0; i < cantidad_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < cantidad_threads; i++) {
        delete argumentos[i];
    }
    sem_destroy(&semaforo);
}

void *procedimientoBrightness(void *argumento) {
    ArgumentosMT *estructura_argumentos = (ArgumentosMT *)argumento;

    sem_wait(estructura_argumentos->semaforo);
    const int ancho_imagen = estructura_argumentos->primera_imagen->ancho;
    const int alto_imagen = estructura_argumentos->primera_imagen->alto;
    sem_post(estructura_argumentos->semaforo);

    for (int fila = estructura_argumentos->fila_inicial; fila < alto_imagen; fila += estructura_argumentos->cantidad_threads) {
        for (int columna = 0; columna < ancho_imagen; columna++) {
            sem_wait(estructura_argumentos->semaforo);
            Pixel pixel_final = estructura_argumentos->primera_imagen->getPixel(fila, columna);
            sem_post(estructura_argumentos->semaforo);

            pixel_final.sumar(255.f * estructura_argumentos->parametro);
            
            sem_wait(estructura_argumentos->semaforo);
            estructura_argumentos->primera_imagen->setPixel(fila, columna, pixel_final.truncar());
            sem_post(estructura_argumentos->semaforo);
        }
    }

    return nullptr;
}

void brightnessMT(PPM *imagen, const float &intensidad, const int &cantidad_threads) {
    std::vector<pthread_t> threads(cantidad_threads);
    std::vector<ArgumentosMT *> argumentos(cantidad_threads);
    sem_t semaforo;
    sem_init(&semaforo, 0, 1);

    for (int i = 0; i < cantidad_threads; i++) {
        argumentos[i] = new ArgumentosMT(imagen, &semaforo, intensidad, cantidad_threads, i);
        pthread_create(&threads[i], NULL, procedimientoBrightness, argumentos[i]);
    }
    for (int i = 0; i < cantidad_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < cantidad_threads; i++) {
        delete argumentos[i];
    }
    sem_destroy(&semaforo);
}

void *procedimientoContrast(void *argumento) {
    ArgumentosMT *estructura_argumentos = (ArgumentosMT *)argumento;

    sem_wait(estructura_argumentos->semaforo);
    const int ancho_imagen = estructura_argumentos->primera_imagen->ancho;
    const int alto_imagen = estructura_argumentos->primera_imagen->alto;
    sem_post(estructura_argumentos->semaforo);

    float factor_intensidad = (259.f * (estructura_argumentos->parametro + 255.f)) / (255.f * (259.f - estructura_argumentos->parametro));
    auto calculo_intensidad = [factor_intensidad] (unsigned char valor_color) -> short int { return static_cast<short int>(factor_intensidad*(valor_color-128)+128); };
    for (int fila = estructura_argumentos->fila_inicial; fila < alto_imagen; fila += estructura_argumentos->cantidad_threads) {
        for (int columna = 0; columna < ancho_imagen; columna++) {
            sem_wait(estructura_argumentos->semaforo);
            Pixel pixel_final = estructura_argumentos->primera_imagen->getPixel(fila, columna);
            sem_post(estructura_argumentos->semaforo);
            
            pixel_final.rojo = calculo_intensidad(pixel_final.rojo);
            pixel_final.verde = calculo_intensidad(pixel_final.verde);
            pixel_final.azul = calculo_intensidad(pixel_final.azul);
            
            sem_wait(estructura_argumentos->semaforo);
            estructura_argumentos->primera_imagen->setPixel(fila, columna, pixel_final.truncar());
            sem_post(estructura_argumentos->semaforo);
        }
    }

    return nullptr;
}

void contrastMT(PPM *imagen, const float &intensidad, const int &cantidad_threads) {
    std::vector<pthread_t> threads(cantidad_threads);
    std::vector<ArgumentosMT *> argumentos(cantidad_threads);
    sem_t semaforo;
    sem_init(&semaforo, 0, 1);

    for (int i = 0; i < cantidad_threads; i++) {
        argumentos[i] = new ArgumentosMT(imagen, &semaforo, intensidad, cantidad_threads, i);
        pthread_create(&threads[i], NULL, procedimientoContrast, argumentos[i]);
    }
    for (int i = 0; i < cantidad_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < cantidad_threads; i++) {
        delete argumentos[i];
    }
    sem_destroy(&semaforo);
}

void *procedimientoMerge(void *argumento) {
    ArgumentosMT *estructura_argumentos = (ArgumentosMT *)argumento;

    sem_wait(estructura_argumentos->semaforo);
    const int ancho_imagen = estructura_argumentos->primera_imagen->ancho;
    const int alto_imagen = estructura_argumentos->primera_imagen->alto;
    sem_post(estructura_argumentos->semaforo);

    float alfa_primera_imagen = estructura_argumentos->parametro;
    float alfa_segunda_imagen = 1.f - alfa_primera_imagen;

    for (int fila = estructura_argumentos->fila_inicial; fila < alto_imagen; fila += estructura_argumentos->cantidad_threads) {
        for (int columna = 0; columna < ancho_imagen; columna++) {
            sem_wait(estructura_argumentos->semaforo);
            Pixel pixel_primera_imagen = estructura_argumentos->primera_imagen->getPixel(fila, columna);
            Pixel pixel_segunda_imagen = estructura_argumentos->segunda_imagen->getPixel(fila, columna);
            sem_post(estructura_argumentos->semaforo);

            pixel_primera_imagen.multiplicar(alfa_primera_imagen);
            pixel_segunda_imagen.multiplicar(alfa_segunda_imagen);
            pixel_primera_imagen.sumarPixel(pixel_segunda_imagen);

            sem_wait(estructura_argumentos->semaforo);
            estructura_argumentos->primera_imagen->setPixel(fila, columna, pixel_primera_imagen);
            sem_post(estructura_argumentos->semaforo);
        }
    }

    return nullptr;
}

void mergeMT(PPM *primera_imagen, PPM *segunda_imagen, const float &alfa_primera_imagen, const int &cantidad_threads) {
    std::vector<pthread_t> threads(cantidad_threads);
    std::vector<ArgumentosMT *> argumentos(cantidad_threads);
    sem_t semaforo;
    sem_init(&semaforo, 0, 1);

    for (int i = 0; i < cantidad_threads; i++) {
        argumentos[i] = new ArgumentosMT(primera_imagen, &semaforo, alfa_primera_imagen, cantidad_threads, i, segunda_imagen);
        pthread_create(&threads[i], NULL, procedimientoMerge, argumentos[i]);
    }
    for (int i = 0; i < cantidad_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < cantidad_threads; i++) {
        delete argumentos[i];
    }
    sem_destroy(&semaforo);
}

void *procedimientoBoxBlur(void *argumento) {
    ArgumentosMT *estructura_argumentos = (ArgumentosMT *)argumento;

    sem_wait(estructura_argumentos->semaforo);
    const int ancho_imagen = estructura_argumentos->primera_imagen->ancho;
    const int alto_imagen = estructura_argumentos->primera_imagen->alto;
    sem_post(estructura_argumentos->semaforo);

    for (int fila = estructura_argumentos->fila_inicial+1; fila < alto_imagen-1; fila += estructura_argumentos->cantidad_threads) {
        for (int columna = 1; columna < ancho_imagen-1; columna++) {
            Pixel suma;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    sem_wait(estructura_argumentos->semaforo);
                    Pixel pixel_auxiliar = estructura_argumentos->primera_imagen->getPixel(fila+i, columna+j);
                    sem_post(estructura_argumentos->semaforo);

                    suma.sumarPixel(pixel_auxiliar.multiplicar(estructura_argumentos->parametro));
                    
                }
            }
            sem_wait(estructura_argumentos->semaforo);
            estructura_argumentos->segunda_imagen->setPixel(fila, columna, suma.truncar());
            sem_post(estructura_argumentos->semaforo);
        }
    }

    return nullptr;
}

void boxBlurMT(PPM *imagen, const float &intensidad, const int &cantidad_threads) {
    PPM *imagen_auxiliar = (PPM *) mmap(NULL, sizeof(PPM), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    imagen_auxiliar = new PPM(imagen->ancho, imagen->alto);

    std::vector<pthread_t> threads(cantidad_threads);
    std::vector<ArgumentosMT *> argumentos(cantidad_threads);
    sem_t semaforo;
    sem_init(&semaforo, 0, 1);

    for (int i = 0; i < cantidad_threads; i++) {
        argumentos[i] = new ArgumentosMT(imagen, &semaforo, intensidad, cantidad_threads, i, imagen_auxiliar);
        pthread_create(&threads[i], NULL, procedimientoBoxBlur, argumentos[i]);
    }
    for (int i = 0; i < cantidad_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int fila = 0; fila < imagen->alto; fila++) {
        for (int columna = 0; columna < imagen->ancho; columna++) {
            imagen->setPixel(fila, columna, imagen_auxiliar->getPixel(fila, columna));
        }
    }

    for (int i = 0; i < cantidad_threads; i++) {
        delete argumentos[i];
    }
    sem_destroy(&semaforo);
    delete imagen_auxiliar;

}

void *procedimientoEdgeDetection(void *argumento) {
    ArgumentosMT *estructura_argumentos = (ArgumentosMT *)argumento;

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

    sem_wait(estructura_argumentos->semaforo);
    const int ancho_imagen = estructura_argumentos->primera_imagen->ancho;
    const int alto_imagen = estructura_argumentos->primera_imagen->alto;
    sem_post(estructura_argumentos->semaforo);

    for (int fila = estructura_argumentos->fila_inicial+1; fila < alto_imagen-1; fila += estructura_argumentos->cantidad_threads) {
        for (int columna = 1; columna < ancho_imagen-1; columna++) {
            int filtro_vertical;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    sem_wait(estructura_argumentos->semaforo);
                    filtro_vertical += estructura_argumentos->primera_imagen->getPixel(fila+i, columna+j).rojo * kernel_vertical[i+1][j+1];
                    sem_post(estructura_argumentos->semaforo);
                }
            }
            filtro_vertical /= 9;

            int filtro_horizontal;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    sem_wait(estructura_argumentos->semaforo);
                    filtro_horizontal += estructura_argumentos->primera_imagen->getPixel(fila+i, columna+j).rojo * kernel_horizontal[i+1][j+1];
                    sem_post(estructura_argumentos->semaforo);
                }
            }
            filtro_horizontal /= 9;

            short int pitagoras = static_cast<short int>(std::sqrt(std::pow(filtro_vertical, 2) + std::pow(filtro_horizontal, 2)));

            Pixel pixel_final(pitagoras, pitagoras, pitagoras);
            sem_wait(estructura_argumentos->semaforo);
            estructura_argumentos->segunda_imagen->setPixel(fila, columna, pixel_final.truncar());
            sem_post(estructura_argumentos->semaforo);
        }
    }

    return nullptr;
}

void edgeDetectionMT(PPM *imagen, const int &cantidad_threads) {
    PPM *imagen_auxiliar = (PPM *) mmap(NULL, sizeof(PPM), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    imagen_auxiliar = new PPM(imagen->ancho, imagen->alto);
    
    std::vector<pthread_t> threads(cantidad_threads);
    std::vector<ArgumentosMT *> argumentos(cantidad_threads);
    sem_t semaforo;
    sem_init(&semaforo, 0, 1);

    for (int i = 0; i < cantidad_threads; i++) {
        argumentos[i] = new ArgumentosMT(imagen, &semaforo, 0, cantidad_threads, i, imagen_auxiliar);
        pthread_create(&threads[i], NULL, procedimientoEdgeDetection, argumentos[i]);
    }
    for (int i = 0; i < cantidad_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int fila = 0; fila < imagen->alto; fila++) {
        for (int columna = 0; columna < imagen->ancho; columna++) {
            imagen->setPixel(fila, columna, imagen_auxiliar->getPixel(fila, columna));
        }
    }
    for (int i = 0; i < cantidad_threads; i++) {
        delete argumentos[i];
    }
    sem_destroy(&semaforo);
    delete imagen_auxiliar;
}

void *procedimientoSharpen(void *argumento) {
    ArgumentosMT *estructura_argumentos = (ArgumentosMT *)argumento;

    std::vector<std::vector<int>> kernel = {
        { 0, -1, 0 },
        { -1, 5, -1},
        { 0, -1, 0 }
    };

    sem_wait(estructura_argumentos->semaforo);
    const int ancho_imagen = estructura_argumentos->primera_imagen->ancho;
    const int alto_imagen = estructura_argumentos->primera_imagen->alto;
    sem_post(estructura_argumentos->semaforo);

    for (int fila = estructura_argumentos->fila_inicial+1; fila < alto_imagen-1; fila += estructura_argumentos->cantidad_threads) {
        for (int columna = 1; columna < ancho_imagen-1; columna++) {
            Pixel suma;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    sem_wait(estructura_argumentos->semaforo);
                    Pixel pixel_auxiliar = estructura_argumentos->primera_imagen->getPixel(fila+i, columna+j);
                    sem_post(estructura_argumentos->semaforo);
                    suma.sumarPixel(pixel_auxiliar.multiplicar(kernel[i+1][j+1]));
                }
            }
            sem_wait(estructura_argumentos->semaforo);
            estructura_argumentos->segunda_imagen->setPixel(fila, columna, suma.truncar());
            sem_post(estructura_argumentos->semaforo);
        }
    }

    return nullptr;
}

void sharpenMT(PPM *imagen, const int &cantidad_threads) {
    PPM *imagen_auxiliar = (PPM *) mmap(NULL, sizeof(PPM), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    imagen_auxiliar = new PPM(imagen->ancho, imagen->alto);

    std::vector<pthread_t> threads(cantidad_threads);
    std::vector<ArgumentosMT *> argumentos(cantidad_threads);
    sem_t semaforo;
    sem_init(&semaforo, 0, 1);

    for (int i = 0; i < cantidad_threads; i++) {
        argumentos[i] = new ArgumentosMT(imagen, &semaforo, 0, cantidad_threads, i, imagen_auxiliar);
        pthread_create(&threads[i], NULL, procedimientoSharpen, argumentos[i]);
    }
    for (int i = 0; i < cantidad_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int fila = 0; fila < imagen->alto; fila++) {
        for (int columna = 0; columna < imagen->ancho; columna++) {
            imagen->setPixel(fila, columna, imagen_auxiliar->getPixel(fila, columna));
        }
    }

    for (int i = 0; i < cantidad_threads; i++) {
        delete argumentos[i];
    }
    sem_destroy(&semaforo);
    delete imagen_auxiliar;
}


#endif