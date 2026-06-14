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
// Archivo nodo.h: definición de la clase Nodo utilizada por el
//                 algoritmo A* para representar estados de búsqueda.

#ifndef NODO_H
#define NODO_H

/**
 * @brief Representa un nodo dentro del algoritmo A*.
 *
 * Cada nodo almacena su posición dentro del laberinto y los costes
 * utilizados por A*:
 * - g(n): coste acumulado desde el origen.
 * - h(n): estimación heurística hasta la meta.
 * - f(n): coste total estimado.
 */
class Nodo {
 public:
  Nodo(int x = 0, int y = 0, double g = 0.0, double h = 0.0)
      : x_(x), y_(y), g_(g), h_(h) {}

  int x() const { return x_; }
  int y() const { return y_; }

  double g() const { return g_; }
  double h() const { return h_; }

  /**
   * @brief Devuelve el coste total estimado del nodo.
   * @return Valor de f(n) = g(n) + h(n).
   */
  double f() const { return g_ + h_; }

  void setG(double g) { g_ = g; }
  void setH(double h) { h_ = h; }

  /**
   * @brief Compara dos nodos según su posición.
   * @param other Nodo con el que se compara.
   * @return true si ambos ocupan la misma celda.
   */
  bool operator==(const Nodo& other) const {
    return x_ == other.x_ && y_ == other.y_;
  }

 private:
  int x_;      ///< Coordenada fila del nodo.
  int y_;      ///< Coordenada columna del nodo.
  double g_;   ///< Coste acumulado desde el origen.
  double h_;   ///< Coste heurístico hasta la meta.
};

#endif