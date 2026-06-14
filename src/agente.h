// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Inteligencia Artificial
// Curso: 2025-2026
// Práctica 2: Búsqueda A*
//
// Autor: Ignacio Andrés Rivera Barrientos
// Correo: alu0101675053@ull.edu.es
// Fecha: 10/06/2026
// Archivo agente.h: definición de la clase Agente encargada de
//                   ejecutar A* en un entorno dinámico y almacenar
//                   las métricas de cada ejecución.
#ifndef AGENTE_H
#define AGENTE_H

#include <ostream>

#include "laberinto.h"
#include "celda.h"
#include "AStar.h"

/**
 * @brief Estructura que almacena las métricas obtenidas en una ejecución.
 */
struct ResultadoEjecucion {
  bool alcanzado = false;
  double coste = 0.0;
  int pasos = 0;
  int nodos_generados = 0;
  int nodos_inspeccionados = 0;
  int reintentos = 0;
  double obstaculos_medios = 0.0;
};

/**
 * @brief Agente que navega por el laberinto utilizando A*.
 */
class Agente {
 public:
  Agente(Laberinto& lab, Celda inicio, Celda fin);

  ResultadoEjecucion Ejecutar( std::ostream& salida, TipoHeuristica tipo = TipoHeuristica::MANHATTAN);

 private:
  Laberinto& lab_;     ///< Referencia al laberinto.
  Celda pos_actual_;   ///< Posición actual del agente.
  Celda objetivo_;     ///< Casilla objetivo.
  int reintentos_ = 0; ///< Reintentos consecutivos sin éxito.
};

#endif  // AGENTE_H