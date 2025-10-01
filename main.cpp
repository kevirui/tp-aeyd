#include <iostream>
#include <cstdio>
#include <cstring>
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

struct Corredor
{
    char nombreApellido[50];
    char llegada[11];
};

struct Categorias
{
    char carrera[8];
    char categoria[50];
    int ultimaPosicion;
    Corredor corr1;
    Corredor corr2;
    Corredor corr3;
};

struct Reporte
{
    char localidad[40];
    char ciudad[11];
    int totalParticipantes;
    int totalTiempo;
};

void espacio();
double transformar(char tiempo[11]);
std::string transformarHora(double tiempo);
void leerCorredores(RegCorredores array[], int tamanio, string file, bool mostrar);
void leerCiudades(CorredoresCiudad array[], int tamanio);
int calcularRegistros(string archivo);
void separarCarreras(RegCorredores array[], int tamanio);
void mostrarPorCarrera();
void ordenarPorTiempo(RegCorredores array[], int tamanio);
void exportarPodios(Categorias categoriasCl[], int n, Categorias categoriasNS[], int k);
void leerPodios(string archivo);
void reporteCiudades(CorredoresCiudad ciudades[]);
void pausar();

int main()
{
    int n = calcularRegistros("./files/Archivo_corredores_4Refugios.bin");
    cout << n << endl;
    RegCorredores registro[n];
    CorredoresCiudad registro2[n];

    leerCorredores(registro, n, "./files/Archivo_corredores_4Refugios.bin", false);
    separarCarreras(registro, n);
    mostrarPorCarrera();

    /* CIUDADES */
    leerCiudades(registro2, n);
    // reporteCiudades(registro2);

    return 0;
}

void espacio()
{
    cout << endl
         << "-------------------" << endl
         << endl;
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
    if (!(tiempo[0] >= '0' && tiempo[0] <= '9'))
    {
        return 432000;
    }
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
        perror("Error al abrir el archivo");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long tam = ftell(file);
    fclose(file);

    return tam / sizeof(RegCorredores);
}

// RegCorredores array
void leerCorredores(RegCorredores array[], int tamanio, string archivo, bool mostrar = false)
{
    FILE *file = fopen(archivo.c_str(), "rb");
    fread(array, sizeof(RegCorredores), tamanio, file);
    if (mostrar)
    {
        for (int i = 0; i < tamanio; i++)
        {
            cout << array[i].numero << " - " << array[i].nombreApellido << " - " << array[i].categoria << " - " << array[i].genero << " - " << array[i].localidad << " - " << transformar(array[i].llegada) << endl;
        }
        espacio();
    }
    fclose(file);
}

// CorredoresCiudad array[]
void leerCiudades(CorredoresCiudad array[], int tamanio)
{
    FILE *file = fopen("./files/ciudades.bin", "rb");
    if (!file)
    {
        perror("Error al abrir");
        return;
    }
    fread(array, sizeof(CorredoresCiudad), tamanio, file);
    for (int i = 0; i < tamanio; i++)
    {
        cout << array[i].ciudad << endl;
    }
    espacio();
    fclose(file);
}

void separarCarreras(RegCorredores array[], int tamanio)
{
    string categoria;
    FILE *clasica = fopen("./files/clasica.bin", "wb");
    FILE *nonStop = fopen("./files/nonStop.bin", "wb");
    for (int i = 0; i < tamanio; i++)
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

void mostrarPorCarrera()
{
    int n = calcularRegistros("./files/clasica.bin");
    int k = calcularRegistros("./files/nonStop.bin");

    RegCorredores clasica[n];
    RegCorredores nonStop[k];
    Categorias categoriasCl[n] = {};
    Categorias categoriasNS[k] = {};

    leerCorredores(clasica, n, "./files/clasica.bin", false);
    leerCorredores(nonStop, k, "./files/nonStop.bin", false);

    ordenarPorTiempo(clasica, n);
    ordenarPorTiempo(nonStop, k);

    int F = 1;
    int M = 1;
    int pos = 1;
    int cat = 1;
    bool llegada = true;
    const char noTermino[] = "no termino";
    char nulo[] = "    -     ";
    char tiempoPr[] = "    -     ";
    char tiempoAn[] = "    -     ";
    cout << "Carrera Clasica" << endl
         << endl;
    cout << "Pos - Gen - Cat - Nro - Nombre y Apellido - Categoria - Grupo - Genero - Localidad - Tiempo - TiempoComPri - TiempoComAnt" << endl;
    for (int i = 0; i < n; i++)
    {
        if (transformar(clasica[i].llegada) == 432000)
        {
            strcpy(clasica[i].llegada, noTermino);
            llegada = false;
        }
        for (int j = 0; j < n; j++)
        {
            if (strcmp(clasica[i].categoria, categoriasCl[j].categoria) == 0)
            {
                categoriasCl[j].ultimaPosicion++;
                cat = categoriasCl[j].ultimaPosicion;
                if (cat == 1)
                {
                    strcpy(categoriasCl[j].corr1.nombreApellido, clasica[i].nombreApellido);
                    strcpy(categoriasCl[j].corr1.llegada, clasica[i].llegada);
                }
                else if (cat == 2)
                {
                    strcpy(categoriasCl[j].corr2.nombreApellido, clasica[i].nombreApellido);
                    strcpy(categoriasCl[j].corr2.llegada, clasica[i].llegada);
                }
                else if (cat == 3)
                {
                    strcpy(categoriasCl[j].corr3.nombreApellido, clasica[i].nombreApellido);
                    strcpy(categoriasCl[j].corr3.llegada, clasica[i].llegada);
                }
                break;
            }
            else if (categoriasCl[j].categoria[0] == '\0')
            {
                strcpy(categoriasCl[j].carrera, "Clasica");
                strcpy(categoriasCl[j].categoria, clasica[i].categoria);
                categoriasCl[j].ultimaPosicion = 1;
                strcpy(categoriasCl[j].corr1.nombreApellido, clasica[i].nombreApellido);
                strcpy(categoriasCl[j].corr1.llegada, clasica[i].llegada);
                // 'cat' = 1
                break;
            }
        }

        if (i != 0 and llegada == true)
        {
            strcpy(tiempoPr, transformarHora(transformar(clasica[i].llegada) - transformar(clasica[0].llegada)).c_str());
            strcpy(tiempoAn, transformarHora(transformar(clasica[i].llegada) - transformar(clasica[i - 1].llegada)).c_str());
        }
        else if (!llegada)
        {
            strcpy(tiempoPr, nulo);
            strcpy(tiempoAn, nulo);
        }
        if (clasica[i].genero == 'F')
        {
            cout << pos++ << " - " << F++ << " - " << cat << " - " << clasica[i].numero << " - " << clasica[i].nombreApellido << " - " << clasica[i].categoria << " - " << clasica[i].genero << " - " << clasica[i].localidad << " - " << clasica[i].llegada << " - " << tiempoPr << " - " << tiempoAn << endl;
        }
        else
        {
            cout << pos++ << " - " << M++ << " - " << cat << " - " << clasica[i].numero << " - " << clasica[i].nombreApellido << " - " << clasica[i].categoria << " - " << clasica[i].genero << " - " << clasica[i].localidad << " - " << clasica[i].llegada << " - " << tiempoPr << " - " << tiempoAn << endl;
        }
    }
    espacio();

    for (int i = 0; i < n; i++)
    {
        if (strcmp(categoriasCl[i].categoria, "") != 0)
        {
            cout << categoriasCl[i].categoria << ":" << endl;
            cout << categoriasCl[i].corr1.nombreApellido << " - " << categoriasCl[i].corr1.llegada << endl;
            cout << categoriasCl[i].corr2.nombreApellido << " - " << categoriasCl[i].corr2.llegada << endl;
            cout << categoriasCl[i].corr3.nombreApellido << " - " << categoriasCl[i].corr3.llegada << endl;
            espacio();
        }
    }

    pausar();

    cout << "Carrera Non Stop" << endl
         << endl;
    cout << "Pos - Gen - Cat - Nro - Nombre y Apellido - Categoria - Genero - Localidad - Tiempo - TiempoComPri - TiempoComAnt" << endl;
    F = 1;
    M = 1;
    pos = 1;
    llegada = true;
    cat = 1;

    for (int i = 0; i < k; i++)
    {
        if (transformar(nonStop[i].llegada) == 432000)
        {
            strcpy(nonStop[i].llegada, noTermino);
            llegada = false;
        }
        for (int j = 0; j < k; j++)
        {
            if (strcmp(categoriasNS[j].categoria, nonStop[i].categoria) == 0)
            {
                categoriasNS[j].ultimaPosicion++;
                cout << categoriasNS[j].ultimaPosicion << endl;
                cat = categoriasNS[j].ultimaPosicion;
                if (cat == 1)
                {
                    strcpy(categoriasNS[j].corr1.nombreApellido, nonStop[i].nombreApellido);
                    strcpy(categoriasNS[j].corr1.llegada, nonStop[i].llegada);
                }
                else if (cat == 2)
                {
                    strcpy(categoriasNS[j].corr2.nombreApellido, nonStop[i].nombreApellido);
                    strcpy(categoriasNS[j].corr2.llegada, nonStop[i].llegada);
                }
                else if (cat == 3)
                {
                    strcpy(categoriasNS[j].corr3.nombreApellido, nonStop[i].nombreApellido);
                    strcpy(categoriasNS[j].corr3.llegada, nonStop[i].llegada);
                }
                break;
            }
            else if (categoriasNS[j].categoria[0] == '\0')
            {
                strcpy(categoriasNS[j].carrera, "NonStop");
                strcpy(categoriasNS[j].categoria, nonStop[i].categoria);
                categoriasNS[j].ultimaPosicion = 1;
                strcpy(categoriasNS[j].corr1.nombreApellido, nonStop[i].nombreApellido);
                strcpy(categoriasNS[j].corr1.llegada, nonStop[i].llegada);
                break;
            }
        }
        if (i != 0 and llegada == true)
        {
            strcpy(tiempoPr, transformarHora(transformar(nonStop[i].llegada) - transformar(nonStop[0].llegada)).c_str());
            strcpy(tiempoAn, transformarHora(transformar(nonStop[i].llegada) - transformar(nonStop[i - 1].llegada)).c_str());
        }
        else if (!llegada)
        {
            strcpy(tiempoPr, nulo);
            strcpy(tiempoAn, nulo);
        }
        if (nonStop[i].genero == 'F')
        {
            cout << pos++ << " - " << F++ << " - " << cat << " - " << nonStop[i].numero << " - " << nonStop[i].nombreApellido << " - " << nonStop[i].categoria << " - " << nonStop[i].genero << " - " << nonStop[i].localidad << " - " << nonStop[i].llegada << " - " << tiempoPr << " - " << tiempoAn << endl;
        }
        else
        {
            cout << pos++ << " - " << M++ << " - " << cat << " - " << nonStop[i].numero << " - " << nonStop[i].nombreApellido << " - " << nonStop[i].categoria << " - " << nonStop[i].genero << " - " << nonStop[i].localidad << " - " << nonStop[i].llegada << " - " << tiempoPr << " - " << tiempoAn << endl;
        }
    }

    espacio();

    for (int i = 0; i < k; i++)
    {
        if (strcmp(categoriasNS[i].categoria, "") != 0)
        {
            cout << categoriasNS[i].categoria << ":" << endl;
            cout << categoriasNS[i].corr1.nombreApellido << " - " << categoriasNS[i].corr1.llegada << endl;
            cout << categoriasNS[i].corr2.nombreApellido << " - " << categoriasNS[i].corr2.llegada << endl;
            cout << categoriasNS[i].corr3.nombreApellido << " - " << categoriasNS[i].corr3.llegada << endl;
            espacio();
        }
    }

    pausar();

    exportarPodios(categoriasCl, n, categoriasNS, k);
    leerPodios("./files/podios.bin");
}

void ordenarPorTiempo(RegCorredores array[], int tamanio)
{
    RegCorredores aux;
    for (int i = 0; i < tamanio - 1; i++)
    {
        for (int j = 0; j < tamanio - i - 1; j++)
        {
            if (transformar(array[j].llegada) > transformar(array[j + 1].llegada))
            {
                aux = array[j];
                array[j] = array[j + 1];
                array[j + 1] = aux;
            }
        }
    }
    // for(int i = 0; i < tamanio; i++){
    //     cout << array[i].numero << " - " << array[i].nombreApellido << " - " << array[i].categoria << " - " << array[i].genero << " - " << array[i].localidad << " - " << transformar(array[i].llegada)<< endl;
    // }
    espacio();
}

// Creacion de archivo con los podios

void exportarPodios(Categorias categoriasCl[], int n, Categorias categoriasNS[], int k)
{

    FILE *podios = fopen("./files/podios.bin", "wb");
    if (!podios)
    {
        perror("No se pudo crear podios.bin");
        return;
    }
    for (int i = 0; i < n; i++)
    {
        if (strcmp(categoriasCl[i].categoria, "") != 0)
        {
            fwrite(&categoriasCl[i], sizeof(Categorias), 1, podios);
        }
    }
    for (int i = 0; i < k; i++)
    {
        if (strcmp(categoriasNS[i].categoria, "") != 0)
        {
            fwrite(&categoriasNS[i], sizeof(Categorias), 1, podios);
        }
    }
    fclose(podios);
}

void leerPodios(string archivo)
{
    FILE *verpodios = fopen(archivo.c_str(), "rb");
    if (!verpodios)
    {
        perror("No se pudo abrir podios.bin");
        return;
    }
    Categorias podio;
    while (fread(&podio, sizeof(Categorias), 1, verpodios))
    {
        cout << "Carrera: " << podio.carrera << " | Categoria: " << podio.categoria << endl;
        cout << "  1° " << podio.corr1.nombreApellido << " - " << podio.corr1.llegada << endl;
        cout << "  2° " << podio.corr2.nombreApellido << " - " << podio.corr2.llegada << endl;
        cout << "  3° " << podio.corr3.nombreApellido << " - " << podio.corr3.llegada << endl;
        espacio();
    }

    fclose(verpodios);

    pausar();
}

void pausar()
{
    string pausa;
    cout << "Presione ENTER para continuar...";
    cin >> pausa;
}

// void reporteCiudades(CorredoresCiudad ciudades){
//     int n = calcularRegistros("./files/ciudades.bin");
//     Reporte reporte[n];

// }