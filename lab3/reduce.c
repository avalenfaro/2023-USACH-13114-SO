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

#include "map.h"
#define ROW_LENGHT 1000
#define FILE_SIZE 9924

/**
 * @brief Función que reduce un mapeo de tasaciones e imprime en pantalla el resultado de la correspondiente sumatoria.
 *
 * @param tasaciones    Arreglo de tasaciones
 * @param verbose       Valor que determina si queremos imprimir por consola {0, 1}
 * @param total_lineas  Total de lineas a reducir
 */
void reduce_tasacion(Map *tasaciones, int verbose, int total_lineas)
{
  int v_liviano = 0;
  int carga = 0;
  int transporte_publico = 0;
  for (int i = 0; i < total_lineas; i++)
  {
    v_liviano += tasaciones[i].vehiculo_liviano;
    carga += tasaciones[i].carga;
    transporte_publico += tasaciones[i].transporte_publico;
  }
  if (verbose == 1)
  {
    printf("Total de tasaciones para vehiculo liviano:%d\n", v_liviano);
    printf("Total de tasaciones para vehiculo de carga:%d\n", carga);
    printf("Total de tasaciones para vehiculo de transporte:%d\n\n", transporte_publico);
  }
}

/**
 * @brief Función que reduce un mapeo de valores_pagados e imprime en pantalla el resultado de la correspondiente sumatoria.
 *
 * @param valor_pagado  Arreglo de valor_pagado
 * @param verbose       Valor que determina si queremos imprimir por consola {0, 1}
 * @param total_lineas  Total de lineas a reducir
 */
void reduce_valor_pagado(Map *valor_pagado, int verbose, int total_lineas)
{
  int v_liviano = 0;
  int carga = 0;
  int transporte_publico = 0;
  for (int i = 0; i < total_lineas; i++)
  {
    v_liviano += valor_pagado[i].vehiculo_liviano;
    carga += valor_pagado[i].carga;
    transporte_publico += valor_pagado[i].transporte_publico;
  }

  if (verbose == 1)
  {
    printf("Valor pagado total para vehiculo liviano:%d\n", v_liviano);
    printf("Valor pagado total para vehiculo de carga:%d\n", carga);
    printf("Valor pagado total para vehiculo de transporte:%d\n\n", transporte_publico);
  }
}

/**
 * @brief Función que reduce un mapeo de puertas e imprime en pantalla el resultado de la correspondiente sumatoria.
 *
 * @param puertas       Arreglo de puertas
 * @param verbose       Valor que determina si queremos imprimir por consola {0, 1}
 * @param total_lineas  Total de lineas a reducir
 */
void reduce_puertas(Map *puertas, int verbose, int total_lineas)
{
  int v_liviano_2 = 0;
  int v_liviano_4 = 0;
  int carga_2 = 0;
  int carga_4 = 0;
  int trans_publico_2 = 0;
  int trans_publico_4 = 0;
  int trans_publico_5 = 0;
  for (int i = 0; i < total_lineas; i++)
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