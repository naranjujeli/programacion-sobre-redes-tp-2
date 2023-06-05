#include "stdafx.h"
#include "filters.h"

#define ONE_OVER_BILLION 1E-9

using namespace std;

std::map<std::string, int> filtros_disponibles {
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

void aplicarFiltro(PPM &imagen, void (*filtro) ());
void aplicarFiltroMT();

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
    for (auto caracter : palabra) {
        if (caracter == ',' || caracter == '.') {
            if (coma) {
                return false;
            }
            coma = true;
        }
        if (!std::isdigit(caracter)) {
            return false;
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
    [i, argc] (*parametros_se_acabaron) () -> bool { return i == argc };
    int i = 1;
    if (parametros_se_acabaron()) {
        return false;
    }
    while (!parametros_se_acabaron() && existeFiltro((std::string)argv[i]) {
        filtros_a_aplicar.push_back((std::string)argv[i]);
        i++;
    }
    if (parametros_se_acabaron() || !cantidad_filtros) {
        return false;
    }
    if (!esNumero((std::string)argv[i])) {
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
    while (!parametros_se_acabaron() && esNumero((std::string)argv[i]) {
        parametros.push_back(atof(argv[i]));
        i++;
    }
    if (cantidad_parametros_necesarios > 0) {
        return false;
    }
    if (parametros_se_acabaron() || !existeArchivo((std::string)argv[i]) {
        return false;
    }
    ruta_primera_imagen = argv[i];
    i++;
    if (parametros_se_acabaron() || !existeArchivo((std::string)argv[i]) {
        return false;
    }
    ruta_salida = argv[i];
    i++;
    if (std::find(filtros_a_aplicar.begin(), filtros_a_aplicar.end(), "merge") != filtros_a_aplicar.end()) {
        if (parametros_se_acabaron()) {
            return false;
        }
        ruta_segunda_imagen = argv[i];
    }
    return true;
}

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

    PPM primera_imagen(ruta_primera_imagen);
    if (std::find(filtros_a_aplicar.begin(), filtros_a_aplicar.end(), "merge") != filtros_a_aplicar.end()) {
        PPM segunda_imagen(ruta_segunda_imagen);
    }

    cout << "Aplicando filtros"<< endl;

    if (cantidad_threads == 0) {
        while (!filtros_a_aplicar.empty()) {
            std::string filtro_actual = filtros_a_aplicar.back();
            filtros_a_aplicar.pop_back();
            if (filtro_actual == "plain") {
                plain(primera_imagen, (unsigned char)parametros.pop_back());
            } else if (filtro_actual == "blackwhite") {
                blackWhite(primera_imagen);
            } else if (filtro_actual == "shades") {
                // TODO Verificar el primer parámetro no es menor a 2
                shades(primera_imagen, (unsigned char)parametros.pop_back());
            } else if (filtro_actual == "brightness") {
                brightness(primera_imagen, parametros.pop_back());
            } else if (filtro_actual == "contrast") {
                // TODO Limitar el contraste
                contrast(primera_imagen, parametros.pop_back());
            } else if (filtro_actual == "merge") {
                // TODO Limitar porcentaje
                // TODO Verificar igual tamaño
                PPM segunda_imagen(argv[7]);
                merge(primera_imagen, segunda_imagen, parametros.pop_back());
            } else if (filtro_actual == "boxblur") {
                boxBlur(primera_imagen, parametros.pop_back());
            } else if (filtro_actual == "edgedetection") {
                blackWhite(primera_imagen);
                boxBlur(primera_imagen, 3);
                edgeDetection(primera_imagen);
            } else if (filtro_actual == "sharpen") {
                sharpen(primera_imagen);
            }
        }
    } else {
    }

    cout << "Escribiendo imagen" << endl;
    primera_imagen.escribir_imagen(ruta_salida);
        
    cout << "Listo" << endl;
    return 0;
}
