#ifndef MAP_H
#define MAP_H

typedef struct
{
  char grupo_vehiculo[20];
  int tasacion;
  int valor_pagado;
  int puertas;
} Vehiculo;

typedef struct
{
  int vehiculo_liviano;
  int carga;
  int transporte_publico;
} Map;

Map *map_tasaciones(Vehiculo vehiculos[], int total_lineas);
Map *map_valor_pagado(Vehiculo vehiculos[], int total_lineas);
Map *map_puertas(Vehiculo vehiculos[], int total_lineas);

#endif