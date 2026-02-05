#ifndef LABERINTO_H
#define LABERINTO_H

#include <vector>
#include <random>

#include "celda.h"

class Laberinto {
 public:
  Laberinto(const std::string& nombre_archivo);
  void Mostrar() const;
  bool EsTransitable(int x, int y) const;
  void ActualizarDinamico();
  int Filas() const { return filas_; }
  int Columnas() const { return columnas_; }

  Celda& GetCelda(int x, int y) { return grid_[x][y]; }
  const Celda& GetCelda(int x, int y) const { return grid_[x][y]; }

 private:
  int filas_, columnas_;
  std::vector<std::vector<Celda>> grid_;
  double pin_ = 0.5;
  double pout_ = 0.5;
};

#endif
