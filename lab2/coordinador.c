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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "coordinador.h"

#define ROW_LENGHT 1000 /* */
#define FILE_SIZE 9924

#define LECTURA 0
#define ESCRITURA 1

void create_process(int *pid)
{
  *pid = fork();
}

/**
 * @brief Captura los argumentos proporcionados por consola.
 *
 * @param argc
 * @param argv
 * @param Coordinador
 */
void get_flags(int argc, char const *argv[], Coordinador *c)
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
      c->n = atoi(optarg);
      break;
    case 'm':
      c->m = atoi(optarg);
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
 * @param total_lineas  Total de lineas a leer del archivo
 * @warning Esta función NO se ha probado con valores de total_lineas < 0 y total_lineas > 10000.
 */
void read_lines(FILE *fp, Vehiculo *vehiculos, int total_lineas, int start, int end)
{
  char row[FILE_SIZE];
  int vehicle_idx = 0;
  for (int i = 0; fgets(row, FILE_SIZE, fp) != NULL; i++)
  {
    if (i == 0)
    {
      continue;
    }

    if (i > start && i <= end)
    {
      strcpy(vehiculos[vehicle_idx].grupo_vehiculo, find_token(row, 1));
      vehiculos[vehicle_idx].tasacion = atoi(find_token(row, 6));
      vehiculos[vehicle_idx].valor_pagado = atoi(find_token(row, 11));
      vehiculos[vehicle_idx].puertas = atoi(find_token(row, 23));
      vehicle_idx++;
    }

    if (i > end)
    {
      break;
    }
  }
}

/**
 * @brief Función utilitaria para leer las n primeras filas de un arreglo de vehiculos.
 *
 * @param n         Numero de filas
 * @param vehiculos Arreglo de vehiculos
 */
void head_vehiculos(int n, Vehiculo *vehiculos)
{
  for (int i = 0; i < n; i++)
  {
    printf("grupo vehiculo: %s\n", vehiculos[i].grupo_vehiculo);
    printf("tasacion: %i\n", vehiculos[i].tasacion);
    printf("valor pagado: %i\n", vehiculos[i].valor_pagado);
    printf("puertas: %d\n", vehiculos[i].puertas);
    printf("--------------------\n");
  }
}

void divide_array(int workers, int worker_number, int *chunk_position, int file_size)
{
  int chunk_size = file_size / workers;
  int start = worker_number * chunk_size;
  int end = start + chunk_size;

  chunk_position[0] = start;
  chunk_position[1] = end;
}

int main(int argc, char const *argv[])
{
  Coordinador coordinador;
  get_flags(argc, argv, &coordinador);
  int pipes[coordinador.n][2];

  for (int i = 0; i < coordinador.n; i++)
  {
    if (pipe(pipes[i]) == -1)
    {
      perror("Error al crear el pipe");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < coordinador.n; i++)
  {
    fflush(stdout);
    pid_t pid = fork();

    if (pid == 0)
    {
      if (close(pipes[i][ESCRITURA]) == -1)
      {
        perror("Error en close");
        exit(EXIT_FAILURE);
      }

      if (dup2(pipes[i][LECTURA], STDIN_FILENO) == -1)
      {
        perror("Falló dup2");
        exit(EXIT_FAILURE);
      }

      char *argv[] = {NULL};

      char file_size[100];
      char chunk_size[100];
      char worker_id[100];

      snprintf(file_size, sizeof file_size, "%d", coordinador.total_lineas);
      snprintf(chunk_size, sizeof chunk_size, "%d", coordinador.total_lineas / coordinador.n);
      snprintf(worker_id, sizeof worker_id, "%d", i);

      char *envp[] = {file_size, chunk_size, worker_id, NULL};

      if (execve("./map", argv, envp) == -1)
      {
        perror("Falló exceve");
        exit(EXIT_FAILURE);
      }

      exit(EXIT_SUCCESS);
    }
    else if (pid < 0)
    {
      perror("Error en fork:");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < coordinador.n; i++)
  {
    int chunk[2];
    Vehiculo *vehiculos = (Vehiculo *)malloc(sizeof(Vehiculo *) * coordinador.total_lineas);

    FILE *file = read_file(coordinador.nombre_archivo);
    divide_array(coordinador.n, i, chunk, coordinador.total_lineas);
    read_lines(file, vehiculos, coordinador.total_lineas, chunk[0], chunk[1]);

    write(pipes[i][ESCRITURA], vehiculos, sizeof(Vehiculo *) * coordinador.total_lineas);
    close(pipes[i][ESCRITURA]); // Cerrar el extremo de escritura del pipe en el padre

    free(vehiculos);
  }

  // Esperar a que todos los hijos terminen
  for (int i = 0; i < coordinador.n; i++)
  {
    wait(0);
  }

  for (int i = 0; i < coordinador.m; i++)
  {
    fflush(stdout);

    pid_t pid = fork();
    if (pid == 0)
    {

      printf("Hola");
      char *argv[] = {NULL};

      int chunk[2];

      divide_array(coordinador.m, i, chunk, coordinador.total_lineas);

      char start[100];
      char end[100];
      char chunk_size[1000];
      char verbose[100];
      char worker_number[100];

      snprintf(start, sizeof start, "%d", chunk[0]);
      snprintf(end, sizeof end, "%d", chunk[1]);
      snprintf(chunk_size, sizeof chunk_size, "%d", coordinador.total_lineas / coordinador.m);
      snprintf(verbose, sizeof verbose, "%d", coordinador.verbose);
      snprintf(worker_number, sizeof worker_number, "%d", i);

      char *envp[] = {start, end, chunk_size, verbose, worker_number, NULL};

      if (execve("./reduce", argv, envp) == -1)
      {
        perror("execve");
        exit(EXIT_FAILURE);
      }
    }
    else if (pid < 0)
    {
      perror("Error en fork:");
      exit(EXIT_FAILURE);
    }
  }

  // Esperar a que todos los hijos terminen
  for (int i = 0; i < coordinador.m; i++)
  {
    wait(0);
  }

  return 0;
}
