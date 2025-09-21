#include <iostream>
#include <cstdio>
#include "funciones.h"
using namespace std;

string transformarHora(double tiempo){
  float segundosEnteros = tiempo;
  int segundosInt = tiempo;

  int horas = segundosEnteros / 3600;

  int minutos = (segundosEnteros / 60) - (horas * 60);

  int segundos = ((segundosEnteros - (horas*3600)) - (minutos*60));

  float decimal = (tiempo - segundosInt) * 10;

  char tiempoCad[11] = {"HH:MM:SS.D"};

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

double transformar(char tiempo[11]){
  float horas = ((tiempo[0]-48) * 10) + (tiempo[1]-48);
  float minutos = ((tiempo[3]-48) * 10) + (tiempo[4]-48);
  float segundos = ((tiempo[6]-48) * 10) + (tiempo[7]-48);
  float decima = (tiempo[9]-48);

  double totalSegundos = horas*3600 + minutos*60 + segundos + decima * 0.1;
  return totalSegundos;
}

//RegCorredores array
void leerCorredores(RegCorredores& array[]){
    FILE* file = fopen("./Archivo_corredores_4Refugios.bin", "rb");
    int n = 322;
    fread(array, sizeof(RegCorredores), n, file);
    // for (int i = 0; i < n; i++){
    //     cout << array[i].categoria << endl;
    // }
    // cout << "------------------" << endl;
    RegCorredores temp;
    int i = 1;
    bool okey;
    okey = fread(&temp, sizeof(RegCorredores), 1, file);
    while(okey){
        cout << temp.categoria<< endl;
        cout << i << endl;
        i++;
        okey = fread(&temp, sizeof(RegCorredores), 1, file);
    };
    cout << "-------------------" << endl;
    fclose(file);
}
//CorredoresCiudad array[]
void leerCiudades(CorredoresCiudad& array[]){
    FILE* file = fopen("./ciudades.bin", "rb");
    if(!file){
        perror("Error al abrir");
        return;
    }
    int n = 322;
    fread(array, sizeof(CorredoresCiudad), n, file);
    for (int i = 0; i < n; i++){
        cout << array[i].ciudad << endl;
    }
    cout << "------------------" << endl;
    // CorredoresCiudad temp;
    // int i = 1;
    // bool okey = true;
    // okey = fread(&temp, sizeof(CorredoresCiudad), 1, file);
    
    // while(okey){
    //     cout << temp.ciudad<< endl;
    //     cout << i << endl;
    //     i++;
    //     okey = fread(&temp, sizeof(CorredoresCiudad), 1, file);
    // }
    fclose(file);
}
