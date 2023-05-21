#ifndef PPM_H
#define PPM_H

using namespace std;


struct Pixel{
    short int rojo;
    short int verde;
    short int azul;

    Pixel(short int rojo=0, short int verde=0, short int azul=0) : rojo(rojo), verde(verde), azul(azul) {}

    Pixel& sumar(short int n) { rojo += n; verde += n; azul += n; return *this; }

    Pixel& restar(short int n) { rojo -= n; verde -= n; azul -= n; return *this; }

    Pixel& multiplicar(float n) { rojo *= n; verde *= n; azul *= n; return *this; }

    Pixel& sumarPixel(Pixel otro_pixel) { rojo += otro_pixel.rojo; verde += otro_pixel.verde; azul += otro_pixel.azul; return *this; }

    const short int sumaCumulativa() { return rojo + verde + azul; } 

    Pixel& truncar() { 
       rojo = (rojo > 255) ? 255 : ((rojo < 0) ? 0: rojo); 
       verde = (verde > 255) ? 255 : ((verde < 0) ? 0: verde); 
       azul = (azul > 255) ? 255 : ((azul < 0) ? 0: azul);
       return *this;
    }
};

class PPM {
    private:
        void inicializar(unsigned int ancho, unsigned int alto);

        unsigned int cantidad_filas;
        unsigned int cantidad_columnas;
        std::vector<std::vector<Pixel>> bitmap;

    public:
        unsigned int ancho;
        unsigned int alto;
        unsigned int valor_maximo_colores;
        unsigned int tamanio;

        PPM();
        PPM(const std::string &ruta);
        PPM(unsigned int ancho, unsigned int alto);

        void leer_imagen(const std::string &ruta);
        void escribir_imagen(const std::string &ruta);

        void setPixel(int i, int j, Pixel nuevo_valor) { bitmap[i][j] = nuevo_valor; }
        Pixel getPixel(int i, int j) { return bitmap[i][j]; }
};

#endif