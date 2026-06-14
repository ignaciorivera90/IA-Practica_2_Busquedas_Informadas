#ifndef LABERINTO_H
#define LABERINTO_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "celda.h"
#include "nodo.h"

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

 private:
  int filas_ = 0;
  int columnas_ = 0;
  std::vector<std::vector<Celda>> grid_;

  double pin_ = 0.5;
  double pout_ = 0.5;
};

#endif