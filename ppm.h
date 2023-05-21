#ifndef PPM_H
#define PPM_H

using namespace std;


struct Pixel{

    short int  r;
    short int  g;
    short int  b;

    // Constructor 
    Pixel(unsigned short int r2=0, unsigned short int g2=0, unsigned short int b2 = 0) : r(r2), g(g2), b(b2) {}

    Pixel& add(short int k){ r += k; g += k; b += k; return *this; }

    Pixel& sub(short int k){ r -= k; g -= k; b -= k; return *this; }

    Pixel& mult(float k){ r *= k; g *= k; b *= k; return *this; }

    Pixel& addp(Pixel p){ r += p.r; g += p.g; b += p.b; return *this; }

    short int cumsum(){ return r + g + b; } 

    Pixel& truncate(){ 
       r = (r > 255) ? 255 : ((r < 0) ? 0: (unsigned char)r); 
       g = (g > 255) ? 255 : ((g < 0) ? 0: (unsigned char)g); 
       b = (b > 255) ? 255 : ((b < 0) ? 0: (unsigned char)b);
       return *this;
    }
};


class ppm {

private:

    void init(int _width, int _height);

    unsigned int nr_lines;
    unsigned int nr_columns;
    vector<vector<Pixel>> bitmap;

public:
    
    unsigned int height;
    unsigned int width;
    unsigned int max_col_val;
    unsigned int size;

    ppm();

    // Crea un objeto PPM y lo rellena con la info en fname
    ppm(const std::string &fname);

    // Crea un imagen vacia(con cero) con altura _height y ancho _width
    ppm(int _width, int _height);

    // Lee la imagen PPM de fname
    void read(const std::string &fname);

    // Escribe la imagen PPM de fname
    void write(const std::string &fname);

    void setPixel(int i, int j, Pixel p){ bitmap[i][j] = p; }
  
    Pixel getPixel(int i, int j){ return bitmap[i][j]; }

};

#endif