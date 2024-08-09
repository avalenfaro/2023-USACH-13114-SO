/**
 * @file      map.c
 * @author    Álvaro Valenzuela A.
 * @brief     Archivo que se encarga de mapear un listado de vehiculos a un tipo de estructura Map.
 * @version   0.1
 * @date      2023-05-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "map.h"

#define ROW_LENGHT 1000
#define FILE_SIZE 9924

const char *VEHICULO_LIVIANO_KEY = "Vehiculo Liviano";
const char *CARGA_KEY = "Carga";
const char *TRANSPORTE_PUBLICO_KEY = "Transporte Publico";

/**
 * @brief Mapea un arreglo de vehiculos a una estructura Map en donde solo se almacenarán los valores de tasaciones para vehiculos de tipo liviano, carga y transporte publico.
 *
 * @param vehiculos     Arreglo de vehiculos
 * @param total_lineas  Total de lineas a mapear
 * @return Map*
 */
Map *map_tasaciones(Vehiculo vehiculos[], int total_lineas)
{
  Map *map_tasacion = (Map *)malloc(sizeof(Map) * total_lineas);
  for (int i = 0; i < total_lineas; i++)
  {
    if (strcmp(vehiculos[i].grupo_vehiculo, VEHICULO_LIVIANO_KEY) == 0) // Filtramos solo los vehiculos con llave "Vehiculo Liviano"
    {
      map_tasacion[i].vehiculo_liviano = vehiculos[i].tasacion;
    }

    if (strcmp(vehiculos[i].grupo_vehiculo, CARGA_KEY) == 0) // Filtramos solo los vehiculos con llave "Carga"
    {
      map_tasacion[i].carga = vehiculos[i].tasacion;
    }

    if (strcmp(vehiculos[i].grupo_vehiculo, TRANSPORTE_PUBLICO_KEY) == 0) // Filtramos solo los vehiculos con llave "Transporte publico"
    {
      map_tasacion[i].transporte_publico = vehiculos[i].tasacion;
    }
  }

  return map_tasacion;
}

/**
 * @brief Mapea un arreglo de vehiculos a una estructura Map en donde solo se almacenarán los valores de valor_pagado para vehiculos de tipo liviano, carga y transporte publico.
 *
 * @param vehiculos Arreglo de vehiculos
 * @param total_lineas Total de lineas a mapear
 * @return Map*
 */
Map *map_valor_pagado(Vehiculo vehiculos[], int total_lineas)
{
  Map *map_valor_pagado = (Map *)malloc(sizeof(Map) * total_lineas);
  for (int i = 0; i < total_lineas; i++)
  {
    if (strcmp(vehiculos[i].grupo_vehiculo, VEHICULO_LIVIANO_KEY) == 0) // Filtramos solo los vehiculos con llave "Vehiculo Liviano"
    {
      map_valor_pagado[i].vehiculo_liviano = vehiculos[i].valor_pagado;
    }

    if (strcmp(vehiculos[i].grupo_vehiculo, CARGA_KEY) == 0) // Filtramos solo los vehiculos con llave "Carga"
    {
      map_valor_pagado[i].carga = vehiculos[i].valor_pagado;
    }

    if (strcmp(vehiculos[i].grupo_vehiculo, TRANSPORTE_PUBLICO_KEY) == 0) // Filtramos solo los vehiculos con llave "Transporte publico"
    {
      map_valor_pagado[i].transporte_publico = vehiculos[i].valor_pagado;
    }
  }

  return map_valor_pagado;
}

/**
 * @brief Mapea un arreglo de vehiculos a una estructura Map en donde solo se almacenarán los valores de grupo_vehiculo para vehiculos de tipo liviano, carga y transporte publico.
 *
 * @param vehiculos Arreglo de vehiculos
 * @param total_lineas Total de lineas a mapear
 * @return Map*
 */
Map *map_puertas(Vehiculo vehiculos[], int total_lineas)
{
  Map *map_puertas = (Map *)malloc(sizeof(Map) * total_lineas);
  for (int i = 0; i < total_lineas; i++)
  {
    if (strcmp(vehiculos[i].grupo_vehiculo, VEHICULO_LIVIANO_KEY) == 0) // Filtramos solo los vehiculos con llave "Vehiculo Liviano"
    {
      map_puertas[i].vehiculo_liviano = vehiculos[i].puertas;
    }

    if (strcmp(vehiculos[i].grupo_vehiculo, CARGA_KEY) == 0) // Filtramos solo los vehiculos con llave "Carga"
    {
      map_puertas[i].carga = vehiculos[i].puertas;
    }

    if (strcmp(vehiculos[i].grupo_vehiculo, TRANSPORTE_PUBLICO_KEY) == 0) // Filtramos solo los vehiculos con llave "Transporte publico"
    {
      map_puertas[i].transporte_publico = vehiculos[i].puertas;
    }
  }

  return map_puertas;
}
