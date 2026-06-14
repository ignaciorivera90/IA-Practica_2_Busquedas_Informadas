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
// Archivo laberinto.h: definición de la clase Laberinto, encargada de
//                      almacenar el mapa, gestionar obstáculos dinámicos
//                      y representar caminos.

#ifndef LABERINTO_H
#define LABERINTO_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "celda.h"
#include "nodo.h"

/**
 * @brief Representa el laberinto sobre el que se ejecuta A*.
 *
 * Almacena la matriz de celdas, las paredes originales del mapa y
 * los parámetros usados para actualizar obstáculos dinámicos.
 */
class Laberinto {
 public:
  Laberinto() = default;
  Laberinto(const std::string& nombre_archivo);

  void Cargar(const std::string& nombre_archivo);
  void Mostrar() const;
  void MostrarConCamino(const std::vector<Nodo>& camino) const;
  void EscribirConCamino(const std::vector<Nodo>& camino, std::ostream& os) const;

  bool EsTransitable(int x, int y) const;
  void ActualizarDinamico();

  int Filas() const { return filas_; }
  int Columnas() const { return columnas_; }

  Celda& GetCelda(int x, int y) { return grid_[x][y]; }
  const Celda& GetCelda(int x, int y) const { return grid_[x][y]; }

  Celda BuscarInicio() const;
  Celda BuscarFin() const;

  double ProporcionObstaculos() const;
  int ContarObstaculosDinamicos() const;
  int MaxObstaculosDinamicos() const;
  void MarcarParedOriginal(int x, int y);

 private:
  bool EsParedOriginal(int x, int y) const;
  void LimitarObstaculosDinamicos();

  int filas_ = 0;       ///< Número de filas del laberinto.
  int columnas_ = 0;    ///< Número de columnas del laberinto.

  std::vector<std::vector<Celda>> grid_;  ///< Matriz de celdas del laberinto.

  /**
   * Matriz auxiliar que indica qué posiciones eran paredes originales.
   * Estas paredes no se modifican durante la actualización dinámica.
   */
  std::vector<std::vector<bool>> paredes_originales_;

  double pin_ = 0.5;   ///< Probabilidad de que una celda libre se bloquee.
  double pout_ = 0.5;  ///< Probabilidad de que un obstáculo dinámico desaparezca.
};

#endif