typedef struct
{
  char *nombre_archivo;
  int total_lineas;
  int verbose;
  int n_hebras;
} Coordinador;

typedef struct
{
  int *bounds;
  int thread_number;
} GlobalThread;

typedef struct
{
  FILE *fp;
  Vehiculo *vehiculos;
  int total_lineas;
} ReadLinesArgs;