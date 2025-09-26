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

---

# ✅ To-Do List

## 1. Lectura y limpieza de datos
- [x] Abrir y leer `Archivo corredores 4Refugios.bin`.
- [ ] Reemplazar códigos de llegada no válidos por “No Terminó”.
- [ ] Separar corredores según carrera (Clásica y NonStop).

## 2. Procesamiento por carrera
- [ ] Calcular **posición general** (ordenar por tiempo).
- [ ] Calcular **posición por género**.
- [ ] Calcular **posición por categoría**.
- [ ] Calcular **tiempo total**, diferencia con el primero y con el anterior.
- [ ] Emitir el listado en el formato pedido.
- [ ] Guardar archivo por carrera con los resultados.

## 3. Podios por categoría
- [ ] Obtener top 3 por categoría en ambas carreras.
- [ ] Generar archivo con podios, ordenado por categoría y posición.

## 4. Reporte por localidad y ciudad
- [ ] Leer `Ciudades.bin`.
- [ ] Relacionar corredores con ciudad/país.
- [ ] Calcular:
  - Cantidad de corredores por ciudad.
  - Tiempo promedio por ciudad.
  - Totales por país.
- [ ] Emitir reporte con formato del ejemplo.

## 5. Entregables
- [ ] Archivos binarios de salida (uno por carrera + podios).
- [ ] Reporte de ciudades/países.
- [ ] Código bien modularizado y comentado.
- [ ] Documento (opcional) explicando implementación.

---

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
