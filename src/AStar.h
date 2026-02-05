#ifndef ASTAR_H
#define ASTAR_H

#include <queue>
#include <map>
#include <cmath>

#include "laberinto.h"
#include "nodo.h"

class AStar {
 public:
  AStar(Laberinto& lab) : laberinto_(lab) {}
  bool BuscarCamino(const Celda& inicio, const Celda& fin, std::vector<Nodo>& camino);

 private:
  double Heuristica(const Nodo& a, const Nodo& b) const;
  double CosteMovimiento(const Nodo& a, const Nodo& b) const;
  std::vector<Nodo> Vecinos(const Nodo& n) const;

  Laberinto& laberinto_;
};


#endif
