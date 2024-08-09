/**
 * @file      coordinador.c
 * @author    Álvaro Valenzuela
 * @brief     Archivo coordinador que orquesta los llamados a las funciones de mapeo y reduce.
 * @version   0.1
 * @date      2023-05-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#define _XOPEN_SOURCE 600 /* Or higher */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "map.h"
#include "reduce.h"
#include "coordinador.h"

#define ROW_LENGHT 1000 /* */
#define FILE_SIZE 9924

// Variables Globales
FILE *fp;
Coordinador *c;

//! NO EXISTE EN MAC
pthread_barrier_t barrera;

/**
 * @brief Captura los argumentos proporcionados por consola.
 *
 * @param argc
 * @param argv
 * @param Coordinador
 */
void get_flags(int argc, char const *argv[])
{
  int opt;
  c->verbose = 0;
  while ((opt = getopt(argc, (char *const *)argv, "i:c:n:m:d")) != -1)
  {
    switch (opt)
    {
    case 'i':
      c->nombre_archivo = optarg;
      break;
    case 'c':
      c->total_lineas = atoi(optarg);
      break;
    case 'd':
      c->verbose = 1;
      break;
    case 'n':
      c->n_hebras = atoi(optarg);
      break;
    case '?':
      printf("No existe el flag %c\n", optopt);
      break;
    default:
      abort();
    }
  }
}

/**
 * @brief Busca el archivo proporcionado en los argumentos para después leer sus filas.
 *
 * @param nombre_archivo
 * @throw File not found
 * @return FILE*
 */
FILE *read_file(char nombre_archivo[])
{
  FILE *fp = fopen((const char *)nombre_archivo, "r");
  int errnum;
  if (fp == NULL)
  {
    errnum = errno;
    printf("Error al abrir el archivo: %s\n", strerror(errnum));
    exit(-1);
  }

  return fp;
}

/**
 * @brief Busca un token en particular (palabra) en una fila de caracteres.
 * @warning La fila proporcionada en los argumentos debe estar limitada por ;
 * @param row
 * @param col_number
 * @return char*
 */
char *find_token(char *row, int col_number)
{
  char *row_copy = (char *)malloc(ROW_LENGHT);
  char *token;
  strcpy(row_copy, row);
  for (int i = 0; i < col_number; i++)
  {
    if (i == 0)
    {
      token = strtok(row_copy, ";");
      continue;
    }

    token = strtok(NULL, ";");
  }

  return token;
}

/**
 * @brief Lee las lineas de un archivo de texto
 *
 * @param fp            Archivo a leer
 * @param vehiculos     Arreglo vacio para almacenar valores
 * @warning Esta función NO se ha probado con valores de total_lineas < 0 y total_lineas > 10000.
 */
void read_lines(Vehiculo vehiculos[], int start, int end)
{
  char row[FILE_SIZE];
  int i = 0;
  while (fgets(row, FILE_SIZE, fp) != NULL)
  {
    if (i == 0)
    {
      i++;
      continue;
    }

    if (i > c->total_lineas)
    {
      break;
    }
    vehiculos[i - 1].grupo_vehiculo = find_token(row, 1);
    vehiculos[i - 1].tasacion = atof(find_token(row, 6));
    vehiculos[i - 1].valor_pagado = atof(find_token(row, 11));
    vehiculos[i - 1].puertas = atoi(find_token(row, 23));
    i++;
  }
}

/**
 * @brief Función utilitaria para leer las n primeras filas de un arreglo de vehiculos.
 *
 * @param n         Numero de filas
 * @param vehiculos Arreglo de vehiculos
 */
void head_vehiculos(int n, Vehiculo vehiculos[])
{
  for (int i = 0; i < n; i++)
  {
    printf("grupo vehiculo: %s\n", vehiculos[i].grupo_vehiculo);
    printf("tasacion: %.2f\n", vehiculos[i].tasacion);
    printf("valor pagado: %.2f\n", vehiculos[i].valor_pagado);
    printf("puertas: %d\n", vehiculos[i].puertas);
    printf("--------------------\n");
  }
}

/**
 * @brief Función utilitaria para leer las n primeras filas de un arreglo de mapeos.
 *
 * @param n   Numero de filas
 * @param map Arreglo de mapeos
 */
void head_mapeo(int n, Map map[])
{
  for (int i = 0; i < n; i++)
  {
    printf("grupo vehiculo: %.2f\n", map[i].vehiculo_liviano);
    printf("carga: %.2f\n", map[i].carga);
    printf("transporte publico: %.2f\n", map[i].transporte_publico);
    printf("--------------------\n");
  }
}

void *thread(void *i)
{
  int thread_n = (int)i;
  printf("%d", thread_n);
  int chunk = c->total_lineas / thread_n;
  int start = 0;
  int end = chunk - 1;
  int thread_idx = 0;

  Vehiculo vehiculos[chunk];

  for (int i = 0; i < thread_n; i++)
  {
    thread_idx = 0;
    read_lines(vehiculos, start, end);
    start += chunk;
    end += chunk;
  }
  pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
  c = (Coordinador *)malloc(sizeof(Coordinador));
  get_flags(argc, argv);
  pthread_t threads_map[c->n_hebras];

  GlobalThread *gt = (GlobalThread *)malloc(sizeof(GlobalThread) * c->n_hebras);
  FILE *file = read_file(c->nombre_archivo);
  Vehiculo vehiculos[c->total_lineas];

  for (int i = 0; i < c->n_hebras; i++)
  {
    pthread_create(&threads_map[i], NULL, thread, (void *)i);
  }

  // read_lines(file, vehiculos, coordinador.total_lineas);
  // Map *tasaciones = map_tasaciones(vehiculos, coordinador.total_lineas);
  // Map *valor_pagado = map_valor_pagado(vehiculos, coordinador.total_lineas);
  // Map *puertas = map_puertas(vehiculos, coordinador.total_lineas);
  // reduce_tasacion(tasaciones, coordinador.verbose, coordinador.total_lineas);
  // reduce_valor_pagado(valor_pagado, coordinador.verbose, coordinador.total_lineas);
  // reduce_puertas(puertas, coordinador.verbose, coordinador.total_lineas);
  return 0;
}
