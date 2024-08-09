/**
 * @file      reduce.c
 * @author    Álvaro Valenzuela A.
 * @brief     Archivo que se encarga de reducir y sumar todos los valores de un arreglo de objetos Map.
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

#include "map.h"
#define ROW_LENGHT 1000
#define FILE_SIZE 9924

void file_create_write_line(char *filename, char *text)
{
  FILE *f;
  f = fopen(filename, "a");

  fprintf(f, "%s", text);
  fclose(f);
}

void write_results(int vehiculo_liviano, int carga, int transporte_publico, char *group_name, int worker_number)
{
  char text[1000];
  snprintf(text, sizeof(char) * 1000, "Total de %s para vehiculo liviano:%i\nTotal de %s para vehiculo de carga:%i\nTotal de %s para vehiculo de transporte:%i\n", group_name, vehiculo_liviano, group_name, carga, group_name, transporte_publico);

  char filename[35];
  snprintf(filename, sizeof(char) * 35, "output_files/worker_%i_output.txt", worker_number);

  file_create_write_line(filename, text);
}

/**
 * @brief Función que reduce un mapeo de tasaciones e imprime en pantalla el resultado de la correspondiente sumatoria.
 *
 * @param tasaciones    Arreglo de tasaciones
 * @param verbose       Valor que determina si queremos imprimir por consola {0, 1}
 * @param total_lineas  Total de lineas a reducir
 */
void reduce_tasacion(Map *tasaciones, int verbose, int chunk_size, int worker_number)
{
  int v_liviano = 0;
  int carga = 0;
  int transporte_publico = 0;
  // printf("%d ", chunk_size);

  for (int i = 0; i < chunk_size; i++)
  {

    v_liviano += tasaciones[i].vehiculo_liviano;
    carga += tasaciones[i].carga;
    transporte_publico += tasaciones[i].transporte_publico;
  }
  char group_name[9] = "tasacion";

  write_results(v_liviano, carga, transporte_publico, group_name, worker_number);
  // if (verbose == 1)
  // {
  //   printf("Total de tasaciones para vehiculo liviano:%d\n", v_liviano);
  //   printf("Total de tasaciones para vehiculo de carga:%d\n", carga);
  //   printf("Total de tasaciones para vehiculo de transporte:%d\n\n", transporte_publico);
  // }
}

/**
 * @brief Función que reduce un mapeo de valores_pagados e imprime en pantalla el resultado de la correspondiente sumatoria.
 *
 * @param valor_pagado  Arreglo de valor_pagado
 * @param verbose       Valor que determina si queremos imprimir por consola {0, 1}
 * @param total_lineas  Total de lineas a reducir
 */
void reduce_valor_pagado(Map *valor_pagado, int verbose, int start, int end, int worker_number)
{
  int v_liviano = 0;
  int carga = 0;
  int transporte_publico = 0;
  for (int i = start; i < end; i++)
  {
    v_liviano += valor_pagado[i].vehiculo_liviano;
    carga += valor_pagado[i].carga;
    transporte_publico += valor_pagado[i].transporte_publico;
  }

  write_results(v_liviano, carga, transporte_publico, "valor_pagado", worker_number);

  if (verbose == 1)
  {
    printf("Valor pagado total para vehiculo liviano:%d\n", v_liviano);
    printf("Valor pagado total para vehiculo de carga:%d\n", carga);
    printf("Valor pagado total para vehiculo de transporte:%d\n\n", transporte_publico);
  }
}

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

void read_lines(FILE *fp, Map *map, int start, int end)
{

  char row[FILE_SIZE];
  int vehicle_idx = 0;
  for (int i = 0; fgets(row, FILE_SIZE, fp) != NULL; i++)
  {
    if (i > start && i <= end)
    {
      map[vehicle_idx].vehiculo_liviano = atoi(find_token(row, 1));
      map[vehicle_idx].carga = atoi(find_token(row, 2));
      map[vehicle_idx].transporte_publico = atoi(find_token(row, 3));

      vehicle_idx++;
    }

    if (i > end)
    {
      break;
    }
  }
}

/**
 * @brief Función que reduce un mapeo de puertas e imprime en pantalla el resultado de la correspondiente sumatoria.
 *
 * @param puertas       Arreglo de puertas
 * @param verbose       Valor que determina si queremos imprimir por consola {0, 1}
 * @param total_lineas  Total de lineas a reducir
 */
void reduce_puertas(Map *puertas, int verbose, int start, int end)
{
  int v_liviano_2 = 0;
  int v_liviano_4 = 0;
  int carga_2 = 0;
  int carga_4 = 0;
  int trans_publico_2 = 0;
  int trans_publico_4 = 0;
  int trans_publico_5 = 0;
  for (int i = start; i < end; i++)
  {
    if (puertas[i].vehiculo_liviano == 2)
    {
      v_liviano_2 += 1;
    }

    if (puertas[i].vehiculo_liviano == 4)
    {
      v_liviano_4 += 1;
    }

    if (puertas[i].carga == 2)
    {
      carga_2 += 1;
    }
    if (puertas[i].carga == 4)
    {
      carga_4 += 1;
    }

    if (puertas[i].transporte_publico == 2)
    {
      trans_publico_2 += 1;
    }
    if (puertas[i].transporte_publico == 4)
    {
      trans_publico_4 += 1;
    }
    if (puertas[i].transporte_publico == 5)
    {
      trans_publico_5 += 1;
    }
  }

  if (verbose == 1)
  {
    printf("Total de vehiculos con 2 puertas para Vehiculos Livianos: %d\n", v_liviano_2);
    printf("Total de vehiculos con 4 puertas para Vehiculos Livianos: %d\n", v_liviano_4);
    printf("Total de vehiculos con 2 puertas para carga: %d\n", carga_2);
    printf("Total de vehiculos con 4 puertas para carga: %d\n", carga_4);
    printf("Total de vehiculos con 2 puertas para Transporte Publico: %d\n", trans_publico_2);
    printf("Total de vehiculos con 4 puertas para Transporte Publico: %d\n", trans_publico_4);
    printf("Total de vehiculos con 5 puertas para Transporte Publico: %d\n", trans_publico_5);
  }
}

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

int main(int argc, char const *argv[])
{
  FILE *tfp = read_file("input_files/tasaciones.csv");
  FILE *vpfp = read_file("input_files/valor_pagado.csv");
  FILE *pfp = read_file("input_files/puertas.csv");

  int start = atoi(argv[1]);
  int end = atoi(argv[2]);
  int chunk_size = atoi(argv[3]);
  int verbose = atoi(argv[4]);
  int worker_number = atoi(argv[5]);

  Map *map_tasaciones = (Map *)malloc(sizeof(Map) * chunk_size);
  Map *map_valor_pagado = (Map *)malloc(sizeof(Map) * chunk_size);
  Map *map_puertas = (Map *)malloc(sizeof(Map) * chunk_size);

  printf("%d ", chunk_size);

  read_lines(tfp, map_tasaciones, start, end);
  read_lines(vpfp, map_valor_pagado, start, end);

  reduce_tasacion(map_tasaciones, verbose, chunk_size, worker_number);
  // reduce_puertas(map_puertas, verbose, start, end, worker_number);
  reduce_valor_pagado(map_valor_pagado, verbose, start, end, worker_number);

  return 0;
}
