#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

struct RegCorredores
{
    int numero;
    char nombreApellido[50];
    char categoria[50];
    char genero;
    char localidad[40];
    char llegada[11];
};

struct CorredoresCiudad
{
    int numero;
    char nombreApellido[50];
    char localidad[40];
    char ciudad[11];
};

double transformar(char tiempo[11]);
std::string transformarHora(double tiempo);
void leerCorredores(RegCorredores array[]);
void leerCiudades(CorredoresCiudad array[]);
int calcularRegistros(string archivo);
void separarCarreras(RegCorredores array[], int tamaño);

int main()
{
    int n = calcularRegistros("./files/archivo_corredores_4Refugios.bin");
    cout << n;
    RegCorredores registro[n];
    CorredoresCiudad registro2[n];

    leerCorredores(registro);
    leerCiudades(registro2);
    separarCarreras(registro, n);
    // double tiempo = 205431.2;
    // cout << transformarHora(tiempo);
    return 0;
}

std::string transformarHora(double tiempo)
{
    float segundosEnteros = tiempo;
    int segundosInt = tiempo;

    int horas = segundosEnteros / 3600;

    int minutos = (segundosEnteros / 60) - (horas * 60);

    int segundos = ((segundosEnteros - (horas * 3600)) - (minutos * 60));

    float decimal = (tiempo - segundosInt) * 10;

    std::string tiempoCad = "HH:MM:SS.D";

    tiempoCad[0] = (horas / 10) + 48;
    tiempoCad[1] = (horas % 10) + 48;
    tiempoCad[2] = ':';
    tiempoCad[3] = (minutos / 10) + 48;
    tiempoCad[4] = (minutos % 10) + 48;
    tiempoCad[5] = ':';
    tiempoCad[6] = (segundos / 10) + 48;
    tiempoCad[7] = (segundos % 10) + 48;
    tiempoCad[8] = '.';
    tiempoCad[9] = decimal + 48;
    tiempoCad[10] = '\0';

    return tiempoCad;
}

double transformar(char tiempo[11])
{
    float horas = ((tiempo[0] - 48) * 10) + (tiempo[1] - 48);
    float minutos = ((tiempo[3] - 48) * 10) + (tiempo[4] - 48);
    float segundos = ((tiempo[6] - 48) * 10) + (tiempo[7] - 48);
    float decima = (tiempo[9] - 48);

    double totalSegundos = horas * 3600 + minutos * 60 + segundos + decima * 0.1;
    return totalSegundos;
}

int calcularRegistros(string archivo)
{
    FILE *file = fopen(archivo.c_str(), "rb");
    if (!file)
    {
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long tam = ftell(file);
    fclose(file);

    return tam / sizeof(RegCorredores);
}

// RegCorredores array
void leerCorredores(RegCorredores array[], int tamaño)
{
    string txt;
    FILE *file = fopen("./files/archivo_corredores_4Refugios.bin", "rb");
    fread(array, sizeof(RegCorredores), tamaño, file);
    for (int i = 0; i < tamaño; i++)
    {
        txt = string((array[i].categoria));
        cout << txt.substr(11, 7) << endl;
    }
    cout << "-------------------" << endl;
    fclose(file);
}

// CorredoresCiudad array[]
void leerCiudades(CorredoresCiudad array[], int tamaño)
{
    FILE *file = fopen("./files/ciudades.bin", "rb");
    if (!file)
    {
        perror("Error al abrir");
        return;
    }
    fread(array, sizeof(CorredoresCiudad), tamaño, file);
    for (int i = 0; i < tamaño; i++)
    {
        cout << array[i].ciudad << endl;
    }
    cout << "------------------" << endl;
    fclose(file);
}

void separarCarreras(RegCorredores array[], int tamaño)
{
    string categoria;
    FILE *clasica = fopen("./files/clasica.bin", "wb");
    FILE *nonStop = fopen("./files/nonStop.bin", "wb");
    for (int i = 0; i < tamaño; i++)
    {
        categoria = string(array[i].categoria).substr(11, 7);
        if (categoria == "Clasica")
        {
            fwrite(&array[i], sizeof(RegCorredores), 1, clasica);
        }
        else
        {
            fwrite(&array[i], sizeof(RegCorredores), 1, nonStop);
        }
    }
    fclose(clasica);
    fclose(nonStop);
}

// void reemplazarNoValidos(char cadena[])
// {
//     for (int i = 0; i < sizeof(cadena); i++)
//     {
//         if (cadena[i] == '\n' || cadena[i] == '\r')
//         {
//             cadena[i] = 'No Termino';
//         }
//     }
// }