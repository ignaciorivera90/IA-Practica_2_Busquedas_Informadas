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
// Archivo AStar.cc: implementación del algoritmo A* para encontrar
//                   caminos de mínimo coste en el laberinto.

#include "AStar.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <map>
#include <queue>
#include <set>



/**
 * @brief Construye el objeto A* asociado a un laberinto.
 *
 * El algoritmo mantiene una referencia al laberinto para poder consultar
 * si una celda es transitable durante la expansión de nodos.
 */
AStar::AStar(Laberinto& lab) : laberinto_(lab) {}



/**
 * @brief Calcula la estimación heurística entre dos posiciones.
 *
 * Dependiendo del tipo seleccionado, se calcula la distancia Manhattan,
 * Euclídea, Chebyshev o una heurística nula. En las tres primeras se usa
 * un factor multiplicativo 3, tal y como indica el guion de la práctica
 * para la heurística Manhattan.
 */
double AStar::Heuristica(int x1, int y1,
                         int x2, int y2,
                         TipoHeuristica tipo) const {
  int dx = std::abs(x2 - x1);
  int dy = std::abs(y2 - y1);

  switch (tipo) {
    case TipoHeuristica::MANHATTAN:
      return (dx + dy) * 3.0;

    case TipoHeuristica::EUCLIDEA:
      return std::sqrt(dx * dx + dy * dy) * 3.0;

    case TipoHeuristica::CHEBYSHEV:
      return std::max(dx, dy) * 3.0;

    case TipoHeuristica::NULA:
      return 0.0;
  }

  return 0.0;
}



/**
 * @brief Devuelve el coste de desplazarse entre dos celdas vecinas.
 *
 * Los movimientos horizontales y verticales tienen coste 5, mientras que
 * los movimientos diagonales tienen coste 7.
 */
double AStar::CosteMovimiento(int x1, int y1, int x2, int y2) const {
  if (std::abs(x1 - x2) == 1 &&
      std::abs(y1 - y2) == 1) {
    return 7.0;
  }

  return 5.0;
}



/**
 * @brief Ejecuta el algoritmo A* desde una celda inicial hasta una final.
 *
 * Se utiliza una cola de prioridad para almacenar la lista de nodos abiertos,
 * un conjunto para los nodos cerrados, un mapa de costes acumulados g(n) y
 * un mapa de padres para reconstruir el camino al llegar a la meta.
 *
 * @param inicio Celda inicial.
 * @param fin Celda objetivo.
 * @param camino Vector donde se almacena el camino encontrado.
 * @param tipo Heurística utilizada en la función f(n) = g(n) + h(n).
 * @return true si existe camino, false en caso contrario.
 */
bool AStar::BuscarCamino(const Celda& inicio, const Celda& fin, std::vector<Nodo>& camino, TipoHeuristica tipo) {
  coste_final_ = 0.0;
  camino.clear();
  nodos_generados_ = 0;
  nodos_inspeccionados_ = 0;

  // Estado auxiliar almacenado en la cola de prioridad.
  struct Estado {
    double f;  // Coste estimado total f(n) = g(n) + h(n).
    double g;  // Coste acumulado desde el inicio.
    int x;     // Fila de la celda.
    int y;     // Columna de la celda.

    bool operator>(const Estado& other) const {
      return f > other.f;
    }
  };

  // Lista de abiertos: siempre extrae primero el estado con menor f(n).
  std::priority_queue<Estado, std::vector<Estado>, std::greater<Estado>> abiertos;

  // gscore almacena el menor coste conocido para llegar a cada celda.
  std::map<std::pair<int, int>, double> gscore;

  // padres permite reconstruir el camino desde la meta hasta el inicio.
  std::map<std::pair<int, int>, std::pair<int, int>> padres;

  // cerrados contiene las celdas que ya han sido inspeccionadas.
  std::set<std::pair<int, int>> cerrados;

  abiertos.push({ Heuristica(inicio.x(), inicio.y(), fin.x(), fin.y(), tipo), 0, inicio.x(), inicio.y() });

  gscore[{inicio.x(), inicio.y()}] = 0;

  // Desplazamientos posibles: 8 direcciones, incluyendo diagonales.
  const int dx[8] = {-1, -1, -1,  0, 0, 1, 1, 1};
  const int dy[8] = {-1,  0,  1, -1, 1, -1, 0, 1};

  while (!abiertos.empty()) {
    Estado actual = abiertos.top();
    abiertos.pop();

    ++nodos_inspeccionados_;

    // Si se alcanza la meta, se reconstruye el camino usando el mapa de padres.
    if (actual.x == fin.x() && actual.y == fin.y()) {
      coste_final_ = actual.g;

      std::vector<Nodo> inverso;
      std::pair<int, int> pos = {fin.x(), fin.y()};

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

    // Si el nodo ya fue cerrado anteriormente, se descarta.
    if (cerrados.count({actual.x, actual.y})) {
      continue;
    }

    cerrados.insert({actual.x, actual.y});

    // Se generan todos los vecinos transitables del nodo actual.
    for (int i = 0; i < 8; ++i) {
      int nx = actual.x + dx[i];
      int ny = actual.y + dy[i];

      if (!laberinto_.EsTransitable(nx, ny)) {
        continue;
      }

      double nuevo_g = actual.g + CosteMovimiento(actual.x, actual.y, nx, ny);
      auto clave = std::make_pair(nx, ny);

      // Si el vecino no estaba descubierto o se encuentra un mejor camino,
      // se actualiza su coste y su padre.
      if (!gscore.count(clave) || nuevo_g < gscore[clave]) {
        gscore[clave] = nuevo_g;
        padres[clave] = {actual.x, actual.y};

        double h = Heuristica(nx, ny, fin.x(), fin.y(), tipo);

        abiertos.push({ nuevo_g + h, nuevo_g, nx, ny });
        ++nodos_generados_;
      }
    }
  }

  // Si la lista de abiertos queda vacia, no existe camino hasta la meta.
  return false;
}