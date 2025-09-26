# 📌 Resumen del Enunciado
El Trabajo Práctico Integrador de AyED – 4 Refugios consiste en procesar datos de las
carreras “4 Refugios Clásica” y “4 Refugios NonStop” (máx. 1000 corredores por carrera).

- Archivo principal: `Archivo corredores 4Refugios.bin`
  - Contiene datos de cada corredor (número, nombre, categoría, género, localidad, llegada).
- Objetivo: generar reportes con posiciones generales, por género y categoría, tiempos y diferencias.
- Códigos de llegada (`DNF`, `DNF (NL)`, `DSQ (FE)`) deben reemplazarse por **“No Terminó”**.
- Archivo secundario: `Ciudades.bin`, con información de ciudades/países de cada corredor.
- Reportes a generar:
  1. Listado general de cada carrera con cálculos de posiciones y tiempos.
  2. Archivo con podio (top 3) de cada categoría en ambas carreras.
  3. Reporte estadístico por localidad y país: cantidad de corredores y tiempos promedio.

# ⚠️ Consideraciones Importantes
- Mantener los `struct` originales, no modificar tamaños de campos.
- Usar funciones de ordenamiento reutilizables (tiempo, género, categoría).
- Convertir tiempos `HH:MM:SS.D` a valor numérico (segundos con décimas) para cálculos.
- Excluir “No Terminó” de posiciones y promedios.
- Cuidar formato exacto en archivos de salida (para corrección automática).
- Usar arrays estáticos de tamaño máx. 1000.
- Dividir el código en módulos claros: lectura, ordenamiento, cálculos, reportes.

# Compilar los archivos juntos

En terminal:

> g++ main.cpp funciones.cpp -o programa


Y luego ejecutás:

> ./programa
