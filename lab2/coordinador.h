typedef struct
{
  char *nombre_archivo;
  int total_lineas;
  int verbose;
  int n;
  int m;
} Coordinador;

typedef struct
{
  char grupo_vehiculo[20];
  int tasacion;
  int valor_pagado;
  int puertas;
} Vehiculo;