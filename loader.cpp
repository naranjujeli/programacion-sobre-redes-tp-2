#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

void loader(std::string direccion_de_la_carpeta);
bool ends_with(const string& str, const string& suffix);

using namespace std;

int main(std::string filtro, int numero_de_threads, std::string camino_de_la_carpeta){


     if(filtro == "plain"){

     }


    return 0;
}

bool ends_with(const string& str, const string& suffix) {
    if (str.size() < suffix.size()) {
        return false;
    }
    return str.substr(str.size() - suffix.size()) == suffix;
}

void loader(std::string direccion_de_la_carpeta, void (*filtro)()){

    for( const auto& entry : fs::directory_iterator(direccion_de_la_carpeta)){

        if(entry.is_regular_file()){
            std::string camino_absoluto = entry.path().string();

            if(ends_with(camino_absoluto, ".jpg") || ends_with(camino_absoluto, ".jpg") || ends_with(camino_absoluto, ".jpg")){

            
            }
        }
    }
}