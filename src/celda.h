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
// Archivo celda.h: definición de la clase Celda utilizada para
//                  representar cada posición del laberinto.

#ifndef CELDA_H
#define CELDA_H

/**
 * @brief Representa una celda del laberinto.
 *
 * Cada celda almacena sus coordenadas y el tipo de contenido:
 * - 0: casilla libre.
 * - 1: obstáculo.
 * - 3: posición inicial.
 * - 4: posición final.
 */
class Celda {
 public:
  Celda(int x = 0, int y = 0, int tipo = 0) : x_(x), y_(y), tipo_(tipo) {}

  int x() const { return x_; }
  int y() const { return y_; }
  int tipo() const { return tipo_; }

  void setTipo(int t) { tipo_ = t; }

  bool EsLibre() const { return tipo_ != 1; }

 private:
  int x_;     ///< Coordenada fila.
  int y_;     ///< Coordenada columna.

  /**
   * Tipo de celda:
   * 0 -> libre
   * 1 -> obstáculo
   * 3 -> inicio
   * 4 -> fin
   */
  int tipo_;
};

#endif