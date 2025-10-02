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
    float totalTiempo;
};

void espacio();
double transformar(char tiempo[11]);
string transformarHora(double tiempo);
void leerCorredores(RegCorredores array[], int tamanio, string file, bool mostrar);
void leerCiudades(CorredoresCiudad array[], int tamanio);
int calcularRegistrosCorredores(string archivo);
int calcularRegistrosCiudades(string archivo);
void separarCarreras(RegCorredores array[], int tamanio);
void mostrarPorCarrera();
void ordenarPorTiempo(RegCorredores array[], int tamanio);
void exportarPodios(Categorias categoriasCl[], int n, Categorias categoriasNS[], int k);
void leerPodios(string archivo);
void reporteCiudades(CorredoresCiudad ciudades[]);
void procesarCarrera(const char *archivo, const char *nombreCarrera, Categorias categorias[], int maxSize);
void pausar();
void mostrar(Reporte array[], int tamanio);
void ordenarPorLocalidad(Reporte array[], int tamanio);

int main()
{
    int n = calcularRegistrosCorredores("./files/archivo_corredores_4Refugios.bin");
    RegCorredores registro[n];
    CorredoresCiudad registro2[n];
    leerCorredores(registro, n, "./files/archivo_corredores_4Refugios.bin", false);
    separarCarreras(registro, n);
    mostrarPorCarrera();
    leerCiudades(registro2, n);
    reporteCiudades(registro2);

    return 0;
}

void espacio()
{
    cout << endl
         << "-------------------" << endl
         << endl;
}

string transformarHora(double tiempo)
{
    float segundosEnteros = tiempo;
    int segundosInt = tiempo;

    int horas = segundosEnteros / 3600;

    int minutos = (segundosEnteros / 60) - (horas * 60);

    int segundos = ((segundosEnteros - (horas * 3600)) - (minutos * 60));

    float decimal = (tiempo - segundosInt) * 10;

    string tiempoCad = "HH:MM:SS.D";

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

int calcularRegistrosCorredores(string archivo)
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

int calcularRegistrosCiudades(string archivo)
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

    return tam / sizeof(CorredoresCiudad);
}

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

void leerCiudades(CorredoresCiudad array[], int tamanio)
{
    FILE *file = fopen("./files/ciudades.bin", "rb");
    if (!file)
    {
        perror("Error al abrir");
        return;
    }

    size_t leidos = fread(array, sizeof(CorredoresCiudad), tamanio, file);
    fclose(file);

    for (size_t i = 0; i < leidos; i++)
    {
        array[i].localidad[sizeof(array[i].localidad) - 1] = '\0'; // 40-1
        array[i].ciudad[sizeof(array[i].ciudad) - 1] = '\0';       // 11-1
        array[i].nombreApellido[sizeof(array[i].nombreApellido) - 1] = '\0';
    }
    espacio();
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
    int n = calcularRegistrosCorredores("./files/clasica.bin");
    int k = calcularRegistrosCorredores("./files/nonStop.bin");
    Categorias categoriasCl[n] = {};
    Categorias categoriasNS[k] = {};
    procesarCarrera("./files/clasica.bin", "Clasica", categoriasCl, n);
    procesarCarrera("./files/nonStop.bin", "Non Stop", categoriasNS, k);
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
    espacio();
}

void procesarCarrera(const char *archivo, const char *nombreCarrera, Categorias categorias[], int maxSize)
{
    int n = calcularRegistrosCorredores(archivo);
    RegCorredores corredores[n];

    leerCorredores(corredores, n, archivo, false);
    ordenarPorTiempo(corredores, n);

    int F = 1, M = 1, pos = 1, cat = 1;
    bool llegada = true;
    const char noTermino[] = "no termino";
    char nulo[] = "    -     ";
    char tiempoPr[] = "    -     ";
    char tiempoAn[] = "    -     ";

    cout << "Carrera " << nombreCarrera << endl
         << endl;
    cout << "Pos - Gen - Cat - Nro - Nombre y Apellido - Categoria - Genero - Localidad - Tiempo - TiempoComPri - TiempoComAnt" << endl;

    for (int i = 0; i < n; i++)
    {
        if (transformar(corredores[i].llegada) == 432000)
        {
            strcpy(corredores[i].llegada, noTermino);
            llegada = false;
        }

        for (int j = 0; j < maxSize; j++)
        {
            if (strcmp(categorias[j].categoria, corredores[i].categoria) == 0)
            {
                categorias[j].ultimaPosicion++;
                cat = categorias[j].ultimaPosicion;
                if (cat == 1)
                {
                    strcpy(categorias[j].corr1.nombreApellido, corredores[i].nombreApellido);
                    strcpy(categorias[j].corr1.llegada, corredores[i].llegada);
                }
                else if (cat == 2)
                {
                    strcpy(categorias[j].corr2.nombreApellido, corredores[i].nombreApellido);
                    strcpy(categorias[j].corr2.llegada, corredores[i].llegada);
                }
                else if (cat == 3)
                {
                    strcpy(categorias[j].corr3.nombreApellido, corredores[i].nombreApellido);
                    strcpy(categorias[j].corr3.llegada, corredores[i].llegada);
                }
                break;
            }
            else if (categorias[j].categoria[0] == '\0')
            {
                strcpy(categorias[j].carrera, nombreCarrera);
                strcpy(categorias[j].categoria, corredores[i].categoria);
                categorias[j].ultimaPosicion = 1;
                strcpy(categorias[j].corr1.nombreApellido, corredores[i].nombreApellido);
                strcpy(categorias[j].corr1.llegada, corredores[i].llegada);
                break;
            }
        }

        if (i != 0 && llegada == true)
        {
            strcpy(tiempoPr, transformarHora(transformar(corredores[i].llegada) - transformar(corredores[0].llegada)).c_str());
            strcpy(tiempoAn, transformarHora(transformar(corredores[i].llegada) - transformar(corredores[i - 1].llegada)).c_str());
        }
        else if (!llegada)
        {
            strcpy(tiempoPr, nulo);
            strcpy(tiempoAn, nulo);
        }

        if (corredores[i].genero == 'F')
        {
            cout << pos++ << " - " << F++ << " - " << cat << " - " << corredores[i].numero << " - " << corredores[i].nombreApellido << " - " << corredores[i].categoria << " - " << corredores[i].genero << " - " << corredores[i].localidad << " - " << corredores[i].llegada << " - " << tiempoPr << " - " << tiempoAn << endl;
        }
        else
        {
            cout << pos++ << " - " << M++ << " - " << cat << " - " << corredores[i].numero << " - " << corredores[i].nombreApellido << " - " << corredores[i].categoria << " - " << corredores[i].genero << " - " << corredores[i].localidad << " - " << corredores[i].llegada << " - " << tiempoPr << " - " << tiempoAn << endl;
        }
    }

    espacio();

    pausar();
}

void exportarPodios(Categorias categoriasCl[], int n, Categorias categoriasNS[], int k)
{
    const int MAX_PODIOS = 100;
    Categorias allPodios[MAX_PODIOS];
    int count = 0;

    for (int i = 0; i < n && count < MAX_PODIOS; i++)
    {
        if (strcmp(categoriasCl[i].categoria, "") != 0)
        {
            allPodios[count++] = categoriasCl[i];
        }
    }

    for (int i = 0; i < k && count < MAX_PODIOS; i++)
    {
        if (strcmp(categoriasNS[i].categoria, "") != 0)
        {
            allPodios[count++] = categoriasNS[i];
        }
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (strcmp(allPodios[j].categoria, allPodios[j + 1].categoria) > 0)
            {
                Categorias temp = allPodios[j];
                allPodios[j] = allPodios[j + 1];
                allPodios[j + 1] = temp;
                // count++; // Incrementamos count ( no estaba en ningun lado )
            }
        }
    }

    FILE *podios = fopen("./files/podios.bin", "wb");
    if (!podios)
    {
        perror("No se pudo crear podios.bin");
        return;
    }
    for (int i = 0; i < count; i++)
    {
        fwrite(&allPodios[i], sizeof(Categorias), 1, podios);
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
        cout << "Categoria: " << podio.categoria << endl;
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
    cout << "Inserte una letra o numero y presione ENTER para continuar..." << endl;
    cin >> pausa;
}

void ordenarPorLocalidad(Reporte array[], int tamanio)
{
    for (int i = 0; i < tamanio - 1; i++)
    {
        for (int j = 0; j < tamanio - i - 1; j++)
        {

            bool vacioJ = (array[j].localidad[0] == '\0');
            bool vacioJ1 = (array[j + 1].localidad[0] == '\0');

            if (!vacioJ && vacioJ1)
            {
                continue;
            }

            if ((vacioJ && !vacioJ1) ||
                (strcmp(array[j].localidad, array[j + 1].localidad) > 0))
            {
                Reporte aux = array[j];
                array[j] = array[j + 1];
                array[j + 1] = aux;
            }
        }
    }
}

void mostrar(Reporte array[], int tamanio)
{
    char nulo[] = "           ";
    int participantes, total = 0;
    float tiempo = 0;

    for (int i = 0; i < tamanio; i++)
    {
        if (array[i].ciudad[0] == '\0')
        {
            break;
        }
        if (strcmp(array[i].localidad, array[i - 1].localidad) != 0)
        {
            cout << array[i].localidad << " - "
                 << array[i].ciudad << " - "
                 << array[i].totalParticipantes << " - "
                 << transformarHora(array[i].totalTiempo / array[i].totalParticipantes) << endl;
        }
        else
        {
            cout << nulo << " - "
                 << array[i].ciudad << " - "
                 << array[i].totalParticipantes << " - "
                 << transformarHora(array[i].totalTiempo / array[i].totalParticipantes) << endl;
        }
        participantes += array[i].totalParticipantes;
        total += array[i].totalParticipantes;
        tiempo += array[i].totalTiempo;
        if (strcmp(array[i].localidad, array[i + 1].localidad) != 0)
        {
            cout << endl
                 << nulo << "Total participantes: " << participantes << " - Promedio: " << transformarHora(tiempo / participantes) << endl
                 << endl;
            participantes = 0;
            tiempo = 0;
        }
    }
}

void reporteCiudades(CorredoresCiudad ciudades[])
{
    int n = calcularRegistrosCiudades("./files/ciudades.bin");
    int k = calcularRegistrosCorredores("./files/archivo_corredores_4Refugios.bin");
    Reporte reporte[n] = {};
    RegCorredores corredores[k];
    int contador = 0;
    espacio();
    leerCorredores(corredores, k, "./files/archivo_corredores_4Refugios.bin", false);
    for (int j = 0; j < n; j++)
    {
        if (transformar(corredores[j].llegada) == 432000)
        {
            // Segun el profesor, hay que excluir a los que no terminaron
            contador++;
            continue;
        }
        for (int i = 0; i < n; i++)
        {
            if (strcmp(reporte[i].ciudad, ciudades[j].ciudad) == 0)
            {
                reporte[i].totalParticipantes++;
                reporte[i].totalTiempo += transformar(corredores[j].llegada);
                break;
            }
            else if (reporte[i].ciudad[0] == '\0')
            {
                strcpy(reporte[i].localidad, ciudades[j].localidad);
                strcpy(reporte[i].ciudad, ciudades[j].ciudad);
                reporte[i].totalParticipantes = 1;
                reporte[i].totalTiempo = transformar(corredores[j].llegada);
                break;
            }
        }
    }

    espacio();
    ordenarPorLocalidad(reporte, n);
    espacio();
    mostrar(reporte, n);
    espacio();
    pausar();
}