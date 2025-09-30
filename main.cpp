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
void espacio();
double transformar(char tiempo[11]);
std::string transformarHora(double tiempo);
void leerCorredores(RegCorredores array[], int tamaño, string file, bool mostrar);
void leerCiudades(CorredoresCiudad array[], int tamaño);
int calcularRegistros(string archivo);
void separarCarreras(RegCorredores array[], int tamaño);
void mostrarPorCarrera();
void ordenarPorTiempo(RegCorredores array[], int tamaño);

int main()
{
    transformar("02:30:45.3");
    int n = calcularRegistros("./files/Archivo corredores 4Refugios.bin");
    cout << n << endl;
    RegCorredores registro[n];
    // CorredoresCiudad registro2[n];

    leerCorredores(registro, n, "./files/Archivo corredores 4Refugios.bin", false);
    mostrarPorCarrera();
    // leerCiudades(registro2, n);
    // double tiempo = 205431.2;
    // cout << transformarHora(tiempo);
    // separarCarreras(registro, n);

    return 0;
}

void espacio(){
    cout <<endl << "-------------------" << endl << endl;
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
    if (!(tiempo[0] >= '0' && tiempo[0] <= '9')){
        return -1;
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
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long tam = ftell(file);
    fclose(file);

    return tam / sizeof(RegCorredores);
}

// RegCorredores array
void leerCorredores(RegCorredores array[], int tamaño, string archivo, bool mostrar = false)
{
    FILE *file = fopen(archivo.c_str(), "rb");
    fread(array, sizeof(RegCorredores), tamaño, file);
    if(mostrar){
        for (int i = 0; i < tamaño; i++){
            cout << array[i].numero << " - " << array[i].nombreApellido << " - " << array[i].categoria << " - " << array[i].genero << " - " << array[i].localidad << " - " << transformar(array[i].llegada)<< endl;
        }
       espacio();
    }
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
    espacio();
    fclose(file);
}

void separarCarreras(RegCorredores array[], int tamaño){
    string categoria;
    FILE *clasica = fopen("./files/clasica.bin", "wb");
    FILE *nonStop = fopen("./files/nonStop.bin", "wb");
    for(int i = 0; i < tamaño; i++){
        categoria = string(array[i].categoria).substr(11,7);
        if(categoria == "Clasica"){
            fwrite(&array[i], sizeof(RegCorredores), 1, clasica);
        }else{
            fwrite(&array[i], sizeof(RegCorredores), 1, nonStop);
        }
    }
    fclose(clasica);
    fclose(nonStop);
}

void mostrarPorCarrera(){
    int n = calcularRegistros("./files/clasica.bin");
    int k = calcularRegistros("./files/nonStop.bin");

    RegCorredores clasica[n];
    RegCorredores nonStop[k];

    leerCorredores(clasica, n, "./files/clasica.bin", false);
    leerCorredores(nonStop, k, "./files/nonStop.bin", false);

    ordenarPorTiempo(clasica, n);
    ordenarPorTiempo(nonStop, k);

    int F= 1;
    int M= 1;
    int pos = 1;
    const char noTermino[] = "no termino";
    cout << "Carrera Clasica" << endl << endl;
    cout << "Pos - Gen - Nro - Nombre y Apellido - Categoria - Grupo - Genero - Localidad - Tiempo" << endl;
    for(int i = 0; i < n; i++){
        if (transformar(clasica[i].llegada) == -1) {
            for(int j = 0; j < 11; j++){
                clasica[i].llegada[j] = noTermino[j];
            }
        }
        if(clasica[i].genero == 'F'){
            cout << pos++ << " - " << F++  << " - " << clasica[i].numero << " - " << clasica[i].nombreApellido << " - " << clasica[i].categoria << " - " << clasica[i].genero << " - " << clasica[i].localidad << " - " << clasica[i].llegada<< endl;
        }else{
            cout << pos++ << " - " << M++  << " - " << clasica[i].numero << " - " << clasica[i].nombreApellido << " - " << clasica[i].categoria << " - " << clasica[i].genero << " - " << clasica[i].localidad << " - " << clasica[i].llegada<< endl;
        }
    }
    espacio();
    cout << "Carrera Non Stop" << endl << endl;
    cout << "Pos - Gen - Nro - Nombre y Apellido - Categoria - Genero - Localidad - Tiempo" << endl;
    F = 1;
    M = 1;
    pos = 1;
    for(int i = 0; i < k; i++){
        if (!(transformar(nonStop[i].llegada) == -1)) {
            if(nonStop[i].genero == 'F'){
                cout << pos++ << " - " << F++  << " - " << nonStop[i].numero << " - " << nonStop[i].nombreApellido << " - " << nonStop[i].categoria << " - " << nonStop[i].genero << " - " << nonStop[i].localidad << " - " << nonStop[i].llegada<< endl;
            }else{
                cout << pos++ << " - " << M++  << " - " << nonStop[i].numero << " - " << nonStop[i].nombreApellido << " - " << nonStop[i].categoria << " - " << nonStop[i].genero << " - " << nonStop[i].localidad << " - " << nonStop[i].llegada<< endl;
            }
        }
    }
}

void ordenarPorTiempo(RegCorredores array[], int tamaño){
    RegCorredores aux;
    for(int i = 0; i < tamaño - 1; i++){
        for(int j = 0; j < tamaño - i - 1; j++){
            if(transformar(array[j].llegada) < transformar(array[j+1].llegada)){
                aux = array[j];
                array[j] = array[j+1];
                array[j+1] = aux;
            }
        }
    }
    // for(int i = 0; i < tamaño; i++){
    //     cout << array[i].numero << " - " << array[i].nombreApellido << " - " << array[i].categoria << " - " << array[i].genero << " - " << array[i].localidad << " - " << transformar(array[i].llegada)<< endl;
    // }
    espacio();
}