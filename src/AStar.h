#ifndef ASTAR_H
#define ASTAR_H

#include <vector>

#include "laberinto.h"
#include "nodo.h"

class AStar {
 public:
  explicit AStar(Laberinto& lab);

  bool BuscarCamino(const Celda& inicio, const Celda& fin, std::vector<Nodo>& camino);

  int GetGenerados() const { return nodos_generados_; }
  int GetInspeccionados() const { return nodos_inspeccionados_; }

 private:
  double Heuristica(int x1, int y1, int x2, int y2) const;
  double CosteMovimiento(int x1, int y1, int x2, int y2) const;

  Laberinto& laberinto_;

  int nodos_generados_ = 0;
  int nodos_inspeccionados_ = 0;
};

#endif