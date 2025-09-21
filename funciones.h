#ifndef FUNCIONES_H
#define FUNCIONES_H

struct RegCorredores{
    int numero;
    char nombreApellido[50];
    char categoria[50];
    char genero;
    char localidad[40];
    char llegada[11];
};

struct CorredoresCiudad{
    int numero;
    char nombreApellido[50];
    char localidad[40];
    char ciudad[11];
};

double transformar(char tiempo[11]);
string transformarHora(double tiempo);
void leerCorredores(RegCorredores array[]);
void leerCiudades(CorredoresCiudad array[]);

#endif
