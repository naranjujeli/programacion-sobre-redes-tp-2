#include "stdafx.h"
#include "filters.h"

#define ONE_OVER_BILLION 1E-9

using namespace std;

void mostrarAyuda();
bool existeFiltro(const std::string &);
bool existeArchivo(const std::string &);
bool esNumero(const std::string &);
bool guardarParametros(const int &, char *[], std::vector<std::string> &, int &, std::queue<float> &, std::string &, std::string &, std::string &);

std::map<std::string, int> filtros_disponibles { // El número indica la cantidad de parametros que le corresponden a cada filtro
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

int main(int argc , char* argv[]) {

    std::vector<std::string> filtros_a_aplicar;
    int cantidad_threads;
    std::queue<float> parametros;
    std::string ruta_primera_imagen;
    std::string ruta_salida;
    std::string ruta_segunda_imagen;

    if (!guardarParametros(
        argc,
        argv,
        filtros_a_aplicar,
        cantidad_threads,
        parametros,
        ruta_primera_imagen,
        ruta_salida,
        ruta_segunda_imagen
    )) {
        mostrarAyuda();
        return 1;
    }

    PPM *primera_imagen;
    PPM *segunda_imagen;

    std::cout << "Aplicando filtros"<< std::endl;

    if (cantidad_threads == 1) {
        primera_imagen = new PPM(ruta_primera_imagen);
        while (!filtros_a_aplicar.empty()) {
            std::string filtro_actual = filtros_a_aplicar.front();
            filtros_a_aplicar.erase(filtros_a_aplicar.begin()); 
            if (filtro_actual == "plain") {
                plain(primera_imagen, (unsigned char)parametros.front());
                parametros.pop();
            } else if (filtro_actual == "blackwhite") {
                blackWhite(primera_imagen);
            } else if (filtro_actual == "shades") {
                // TODO Verificar el primer parámetro no es menor a 2
                shades(primera_imagen, (unsigned char)parametros.front());
                parametros.pop();
            } else if (filtro_actual == "brightness") {
                brightness(primera_imagen, parametros.front());
                parametros.pop();
            } else if (filtro_actual == "contrast") {
                // TODO Limitar el contraste
                contrast(primera_imagen, parametros.front());
                parametros.pop();
            } else if (filtro_actual == "merge") {
                // TODO Limitar porcentaje
                // TODO Verificar igual tamaño
                segunda_imagen = (PPM *) mmap(NULL, sizeof(PPM), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
                segunda_imagen = new PPM(ruta_segunda_imagen);
                merge(primera_imagen, segunda_imagen, parametros.front());
                munmap(segunda_imagen, sizeof(PPM));
                parametros.pop();
            } else if (filtro_actual == "boxblur") {
                boxBlur(primera_imagen, parametros.front());
                parametros.pop();
            } else if (filtro_actual == "edgedetection") {
                blackWhite(primera_imagen);
                boxBlur(primera_imagen, 3);
                edgeDetection(primera_imagen);
            } else if (filtro_actual == "sharpen") {
                sharpen(primera_imagen);
            }
        }
    } else {
        primera_imagen = (PPM *) mmap(NULL, sizeof(PPM), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        primera_imagen = new PPM(ruta_primera_imagen);
        while (!filtros_a_aplicar.empty()) {
            std::string filtro_actual = filtros_a_aplicar.front();
            filtros_a_aplicar.erase(filtros_a_aplicar.begin()); 
            if (filtro_actual == "plain") {
                plainMT(primera_imagen, (unsigned char)parametros.front(), cantidad_threads);
                parametros.pop();
            } else if (filtro_actual == "blackwhite") {
                blackWhiteMT(primera_imagen, cantidad_threads);
            } else if (filtro_actual == "shades") {
                // TODO Verificar el primer parámetro no es menor a 2
                shadesMT(primera_imagen, (unsigned char)parametros.front(), cantidad_threads);
                parametros.pop();
            } else if (filtro_actual == "brightness") {
                brightnessMT(primera_imagen, parametros.front(), cantidad_threads);
                parametros.pop();
            } else if (filtro_actual == "contrast") {
                // TODO Limitar el contraste
                contrastMT(primera_imagen, parametros.front(), cantidad_threads);
                parametros.pop();
            } else if (filtro_actual == "merge") {
                // TODO Limitar porcentaje
                // TODO Verificar igual tamaño
                segunda_imagen = (PPM *) mmap(NULL, sizeof(PPM), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
                *segunda_imagen = PPM(ruta_segunda_imagen);
                mergeMT(primera_imagen, segunda_imagen, parametros.front(), cantidad_threads);
                munmap(segunda_imagen, sizeof(PPM));
                delete segunda_imagen;
                parametros.pop();
            } else if (filtro_actual == "boxblur") {
                boxBlurMT(primera_imagen, parametros.front(), cantidad_threads);
                parametros.pop();
            } else if (filtro_actual == "edgedetection") {
                blackWhiteMT(primera_imagen, cantidad_threads);
                boxBlurMT(primera_imagen, 3, cantidad_threads);
                edgeDetectionMT(primera_imagen, cantidad_threads);
            } else if (filtro_actual == "sharpen") {
                sharpenMT(primera_imagen, cantidad_threads);
            }
        }
        munmap(primera_imagen, sizeof(PPM));
    }

    cout << "Escribiendo imagen" << endl;
    primera_imagen->escribir_imagen(ruta_salida);
        
    cout << "Listo" << endl;

    delete primera_imagen;

    return 0;
}

void mostrarAyuda() {
    std::cout << "Ha ocurrido un error. Verifique que los parametros sean correctos." << std::endl;
    cout << "Uso: ./main <[filtro]> <cantidad de threads> <[parámetro de filtro]> <primera imagen> <salida> <segunda imagen>" << endl;
}

bool existeFiltro(const std::string &palabra) {
    return filtros_disponibles.find(palabra) != filtros_disponibles.end();
}

bool existeArchivo(const std::string &ruta) {
    ifstream archivo(ruta.c_str());
    return archivo.good();
}

// Verifica que el parámetro sea un número, ya sea entero o con fracción decimal
bool esNumero(const std::string &palabra) { 
    bool coma = false;
    bool signo_menos = false;
    for (auto caracter : palabra) {
        if (!std::isdigit(caracter)) {
            if (caracter == '-') {
                if (signo_menos) {
                    return false;
                }
                signo_menos = true;
            } else if (caracter == ',' || caracter == '.') {
                if (coma) {
                    return false;
                }
                coma = true;
            } else {
                return false;
            }
        }
    }
    return true;
}

bool guardarParametros(
    const int &argc,
    char *argv[],
    std::vector<std::string> &filtros_a_aplicar,
    int &cantidad_threads,
    std::queue<float> &parametros,
    std::string &ruta_primera_imagen,
    std::string &ruta_salida,
    std::string &ruta_segunda_imagen
) {
    int i = 1;
    auto parametros_se_acabaron = [i, argc] () -> bool { return i == argc; };
    if (parametros_se_acabaron()) {
        return false;
    }
    while (!parametros_se_acabaron() && existeFiltro(std::string(argv[i]))) {
        filtros_a_aplicar.push_back((std::string)argv[i]);
        i++;
    }
    if (parametros_se_acabaron() || !filtros_a_aplicar.size()) {
        return false;
    }
    if (!esNumero(std::string(argv[i]))) {
        return false;
    }
    cantidad_threads = atoi(argv[i]);
    i++;
    if (parametros_se_acabaron()) {
        return false;
    }
    int cantidad_parametros_necesarios = 0;
    for (std::string filtro : filtros_a_aplicar) {
        cantidad_parametros_necesarios += filtros_disponibles[filtro];
    }
    while (!parametros_se_acabaron() && esNumero(std::string(argv[i]))) {
        parametros.push(atof(argv[i]));
        i++;
        cantidad_parametros_necesarios--;
    }
    if (cantidad_parametros_necesarios > 0) {
        return false;
    }
    if (parametros_se_acabaron() || !existeArchivo(std::string(argv[i]))) {
        return false;
    }
    ruta_primera_imagen = argv[i];
    i++;
    if (parametros_se_acabaron()) {
        std::cout << "No hay salida" << std::endl;
        return false;
    }
    ruta_salida = argv[i];
    i++;
    if (std::find(filtros_a_aplicar.begin(), filtros_a_aplicar.end(), "merge") != filtros_a_aplicar.end()) {
        if (parametros_se_acabaron()) {
            std::cout << "Error en el merge" << std::endl;
            return false;
        }
        ruta_segunda_imagen = argv[i];
    }
    return true;
}