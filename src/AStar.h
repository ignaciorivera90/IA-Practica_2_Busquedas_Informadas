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
// Archivo AStar.h: definición de la clase AStar encargada de realizar
//                  la búsqueda informada mediante el algoritmo A*.

#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <string>

#include "laberinto.h"
#include "nodo.h"

/**
 * @brief Heurísticas disponibles para el algoritmo A*.
 */
enum class TipoHeuristica {
  MANHATTAN,  ///< Distancia Manhattan.
  EUCLIDEA,   ///< Distancia Euclídea.
  CHEBYSHEV,  ///< Distancia de Chebyshev.
  NULA        ///< Heurística nula (equivalente a Dijkstra).
};

/**
 * @brief Implementación del algoritmo A*.
 *
 * Permite calcular caminos de coste mínimo entre dos celdas
 * utilizando diferentes funciones heurísticas.
 */
class AStar {
 public:
  explicit AStar(Laberinto& lab);

  bool BuscarCamino(const Celda& inicio, const Celda& fin, std::vector<Nodo>& camino, TipoHeuristica tipo = TipoHeuristica::MANHATTAN);

  int GetGenerados() const { return nodos_generados_; }
  int GetInspeccionados() const { return nodos_inspeccionados_; }
  double GetCosteFinal() const { return coste_final_; }

 private:
  double Heuristica(int x1, int y1, int x2, int y2, TipoHeuristica tipo) const;

  double CosteMovimiento(int x1, int y1, int x2, int y2) const;

  Laberinto& laberinto_;  ///< Referencia al laberinto sobre el que opera A*.

  int nodos_generados_ = 0;      ///< Número de nodos generados.
  int nodos_inspeccionados_ = 0; ///< Número de nodos inspeccionados.
  double coste_final_ = 0.0;     ///< Coste del último camino encontrado.
};

#endif