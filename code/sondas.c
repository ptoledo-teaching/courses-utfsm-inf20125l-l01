/*
 * sondas.c — Monitor continuo de sondas imperiales
 *
 * Simula lotes de patrullaje en el Outer Rim. En cada lote, un número
 * aleatorio de sondas detecta actividad rebelde y transmite una alerta
 * (intensidad 0-100). Las sondas que no detectan nada no reportan.
 *
 * ADVERTENCIA: este archivo contiene bugs intencionales para fines
 * demostrativos.
 *
 * Compilar: make
 * Ejecutar: ./sondas
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_SONDAS 10

/**
 * Calcula el promedio de un arreglo de n elementos.
 *
 * @param datos Puntero al arreglo de enteros cuyo promedio se desea calcular.
 * @param n Número de elementos en el arreglo.
 *
 * @return El promedio entero de los elementos del arreglo.
 *         Nota: El resultado es una división entera, por lo que se pierden decimales.
 */
int calcular_promedio(int *datos, int n)
{
  int suma = 0;
  for (int i = 0; i < n; i++)
    suma += datos[i];
  return suma / n;
}

/**
 * calcular_max - Calcula el valor máximo de un arreglo de enteros.
 *
 * @param datos: Puntero al arreglo de enteros.
 * @param n: Número de elementos en el arreglo.
 *
 * @return El valor máximo encontrado en el arreglo.
 */
int calcular_max(int *datos, int n)
{
  int max = 0;
  for (int i = 0; i < n; i++)
    if (datos[i] > max)
      max = datos[i];
  return max;
}

/**
 * calcular_min - Calcula el mínimo de un arreglo de n elementos.
 *
 * @param datos: Puntero al arreglo de enteros.
 * @param n: Número de elementos en el arreglo.
 *
 * @return El valor mínimo encontrado en el arreglo.
 */
int calcular_min(int *datos, int n)
{
  int min = 100;
  for (int i = 0; i < n; i++)
    if (datos[i] < min)
      min = datos[i];
  return min;
}

/**
 * analizar_lote - Simula el análisis de un lote de sondas.
 *
 * @param lote: Número del lote actual.
 * @param n_sondas: Número de sondas patrullando el lote.
 */
void analizar_lote(int lote, int n_sondas)
{
  int *alertas = malloc(n_sondas * sizeof(int));

  /* Simulación: cuántas sondas detectaron actividad rebelde */
  int detecciones = rand() % (n_sondas + 1);

  for (int i = 0; i < detecciones; i++)
    alertas[i] = rand() % 101;

  int avg = calcular_promedio(alertas, detecciones);
  int max = calcular_max(alertas, detecciones);
  int min = calcular_min(alertas, detecciones);

  printf("Lote %7d | sondas: %2d | detecciones: %2d | "
         "max: %3d | min: %3d | avg: %4d\n",
         lote, n_sondas, detecciones, max, min, avg);

  free(alertas);
}

int main(void)
{
  int timer = 0;
  srand(time(NULL));
  printf("=== Monitor de Sondas Imperiales - Sector Outer Rim ===\n\n");

  int lote = 1;
  while (1)
  {
    int n = rand() % MAX_SONDAS + 1;
    analizar_lote(lote++, n);
    sleep(1);
  }

  return 0;
}
