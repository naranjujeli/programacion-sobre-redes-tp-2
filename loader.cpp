#include "stdafx.h"
#include "filters.h"
#include "ppm.h"

#ifndef FILTERS_CPP
#define FILTERS_CPP

namespace fs = std::filesystem;

void loader(std::string direccion_de_la_carpetam, std::function<void()> filtro, int parametro);
bool ends_with(const string& str, const string& suffix);
bool validarCarpeta(const std::string& path);

using namespace std;

std::map<std::string, int> filtros_disponibles { // El numero indica la cantidad de parametros que le corresponden a cada filtro
    { "plain", 0 },
    { "blackwhite", 0 },
    { "shades", 1 },
    { "brightness", 1 },
    { "constrast", 1 },
    { "merge", 1 },
    { "boxblur", 1 },
    { "edgedetection", 0 },
    { "sharpen", 0},
};

bool validarCarpeta(const std::string& path) {
    std::filesystem::path ruta(path);
    return std::filesystem::is_directory(ruta);
}

bool ends_with(const string& str, const string& suffix) {
    if (str.size() < suffix.size()) {
        return false;
    }
    return str.substr(str.size() - suffix.size()) == suffix;
}

int main(int argc, char* argv[]) {
    std::string filtro = argv[1];

    auto iter = filtros_disponibles.find(filtro);
    if(iter == filtros_disponibles.end()){
        cout << "Filtro no disponible o no existente" << endl;
        return 0;
    }

    int cantidad_threads = atoi(argv[2]); // Esto lo convierte a int
    if(cantidad_threads < 1){
        cout << "Valor invalido de cantidad de threads" << endl;
        return 0;
    }

    std::string ruta_carpeta = argv[3];
    if(!validarCarpeta(ruta_carpeta)){
        cout << "Ruta invalida" << endl;
        return 0;
    }

    bool parametro_existe;
    float parametro;
    if(filtros_disponibles[filtro] == 1){
        parametro_existe = true;
        parametro = atof(argv[4]);
    }else{
        parametro_existe = false;
    }

    PPM *primera_imagen;
    PPM *segunda_imagen;

    int i = 0;
    std::cout << "Vamos a iterar la carpeta " << ruta_carpeta << std::endl;
    for( const auto& entry : fs::directory_iterator(ruta_carpeta)) {
        if(fs::is_regular_file(entry)){
            i++;
            std::string ruta_imagen = ruta_carpeta + std::string(entry.path().filename());
            /* if(ruta_imagen == "imgs/ut/.ppm" || ruta_imagen == "imgs/u/.ppm" || ruta_imagen == "imgs/.ppm"){
                continue;
            } */
            if(ends_with(ruta_imagen, ".ppm")) {
                if (cantidad_threads == 1) {
                    std::cout << "Abriendo imagen " << ruta_imagen << std::endl;
                    primera_imagen = new PPM(ruta_imagen);
                    if (filtro == "plain") {
                        plain(primera_imagen, parametro);
                    } else if (filtro == "blackwhite") {
                        blackWhite(primera_imagen);
                    } else if (filtro == "shades") {
                        // TODO Verificar el primer parametro no es menor a 2
                        shades(primera_imagen, parametro);
                    } else if (filtro == "brightness") {
                        brightness(primera_imagen, parametro);
                    } else if (filtro == "contrast") {
                        // TODO Limitar el contraste
                        contrast(primera_imagen, parametro);
                    } else if (filtro == "boxblur") {
                        boxBlur(primera_imagen, parametro);
                    } else if (filtro == "edgedetection") {
                        blackWhite(primera_imagen);
                        boxBlur(primera_imagen, 3);
                        edgeDetection(primera_imagen);
                    } else if (filtro == "sharpen") {
                        sharpen(primera_imagen);
                    }
                } else {
                    primera_imagen = (PPM *) mmap(NULL, sizeof(PPM), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
                    primera_imagen = new PPM(ruta_imagen);
                    if (filtro == "plain") {
                        plainMT(primera_imagen, parametro, cantidad_threads);
                    } else if (filtro == "blackwhite") {
                        blackWhiteMT(primera_imagen, cantidad_threads);
                    } else if (filtro == "shades") {
                        // TODO Verificar el primer parámetro no es menor a 2
                        shadesMT(primera_imagen, parametro, cantidad_threads);
                    } else if (filtro == "brightness") {
                        brightnessMT(primera_imagen, parametro, cantidad_threads);
                    } else if (filtro == "contrast") {
                        // TODO Limitar el contraste
                        contrastMT(primera_imagen, parametro, cantidad_threads);
                    } else if (filtro == "boxblur") {
                        boxBlurMT(primera_imagen, parametro, cantidad_threads);
                    } else if (filtro == "edgedetection") {
                        blackWhiteMT(primera_imagen, cantidad_threads);
                        boxBlurMT(primera_imagen, 3, cantidad_threads);
                        edgeDetectionMT(primera_imagen, cantidad_threads);
                    } else if (filtro == "sharpen") {
                        sharpenMT(primera_imagen, cantidad_threads);
                    }
                    munmap(primera_imagen, sizeof(PPM));
                }
            }
            std::string ruta_salida = std::string("out/" + i) + ".ppm";
            primera_imagen->escribir_imagen(ruta_salida); 
        }
    }

    return 0;
}

#endif