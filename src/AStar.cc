#include "AStar.h"

#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>

AStar::AStar(Laberinto& lab) : laberinto_(lab) {}

double AStar::Heuristica(int x1, int y1, int x2, int y2) const {
  return (std::abs(x2 - x1) + std::abs(y2 - y1)) * 3.0;
}

double AStar::CosteMovimiento(int x1, int y1, int x2, int y2) const {
  if (std::abs(x1 - x2) == 1 && std::abs(y1 - y2) == 1) {
    return 7.0;
  }
  return 5.0;
}

bool AStar::BuscarCamino(const Celda& inicio, const Celda& fin, std::vector<Nodo>& camino) {

  camino.clear();
  nodos_generados_ = 0;
  nodos_inspeccionados_ = 0;

  struct Estado {
    double f;
    double g;
    int x;
    int y;

    bool operator>(const Estado& other) const {
      return f > other.f;
    }
  };

  std::priority_queue<Estado, std::vector<Estado>, std::greater<Estado>> abiertos;

  std::map<std::pair<int,int>, double> gscore;
  std::map<std::pair<int,int>, std::pair<int,int>> padres;
  std::set<std::pair<int,int>> cerrados;

  abiertos.push({ Heuristica(inicio.x(), inicio.y(), fin.x(), fin.y()), 0, inicio.x(), inicio.y() });

  gscore[{inicio.x(), inicio.y()}] = 0;

  const int dx[8] = {-1,-1,-1,0,0,1,1,1};
  const int dy[8] = {-1,0,1,-1,1,-1,0,1};

  while (!abiertos.empty()) {
    Estado actual = abiertos.top();
    abiertos.pop();

    ++nodos_inspeccionados_;

    if (actual.x == fin.x() && actual.y == fin.y()) {

      std::vector<Nodo> inverso;

      std::pair<int,int> pos = {fin.x(), fin.y()};

      while (true) {
        inverso.emplace_back(pos.first, pos.second);

        if (pos.first == inicio.x() &&
            pos.second == inicio.y()) {
          break;
        }

        pos = padres[pos];
      }

      std::reverse(inverso.begin(), inverso.end());
      camino = inverso;
      return true;
    }

    if (cerrados.count({actual.x, actual.y})) {
      continue;
    }

    cerrados.insert({actual.x, actual.y});

    for (int i = 0; i < 8; ++i) {
      int nx = actual.x + dx[i];
      int ny = actual.y + dy[i];

      if (!laberinto_.EsTransitable(nx, ny)) {
        continue;
      }

      double nuevo_g = actual.g + CosteMovimiento(actual.x, actual.y, nx, ny);

      auto clave = std::make_pair(nx, ny);

      if (!gscore.count(clave) ||
          nuevo_g < gscore[clave]) {

        gscore[clave] = nuevo_g;

        padres[clave] = { actual.x, actual.y };

        double h = Heuristica(nx, ny, fin.x(), fin.y());

        abiertos.push({ nuevo_g + h, nuevo_g, nx, ny });
        
        ++nodos_generados_;
      }
    }
  }

  return false;
}