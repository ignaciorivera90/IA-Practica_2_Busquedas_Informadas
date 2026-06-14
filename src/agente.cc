#include "agente.h"

#include <iostream>
#include <vector>

#include "AStar.h"

Agente::Agente(Laberinto& lab, Celda inicio, Celda fin): lab_(lab), pos_actual_(inicio), objetivo_(fin) {}

void Agente::Ejecutar() {
  int pasos = 0;
  int nodos_generados = 0;
  int nodos_inspeccionados = 0;

  double suma_obstaculos = 0.0;
  int iteraciones = 0;

  while (!(pos_actual_.x() == objetivo_.x() && pos_actual_.y() == objetivo_.y())) {

    AStar astar(lab_);

    std::vector<Nodo> camino;

    bool encontrado = astar.BuscarCamino( pos_actual_, objetivo_, camino);

    if (!encontrado) {
      ++reintentos_;

      if (reintentos_ >= 5) {
        std::cout << "Destino inalcanzable tras 5 reintentos.\n";
        return;
      }

      lab_.ActualizarDinamico();
      continue;
    }

    reintentos_ = 0;

    nodos_generados += astar.GetGenerados();
    nodos_inspeccionados += astar.GetInspeccionados();

    std::cout << "\n====================\n";
    std::cout << "Paso " << pasos << "\n";
    lab_.MostrarConCamino(camino);

    if (camino.size() > 1) {
      pos_actual_ = lab_.GetCelda( camino[1].x(), camino[1].y());
      ++pasos;
    }

    suma_obstaculos += lab_.ProporcionObstaculos();
    ++iteraciones;

    lab_.ActualizarDinamico();
  }

  std::cout << "\nMeta alcanzada\n";
  std::cout << "Pasos: " << pasos << "\n";
  std::cout << "Nodos generados: " << nodos_generados << "\n";
  std::cout << "Nodos inspeccionados: " << nodos_inspeccionados << "\n";
  std::cout << "Reintentos: " << reintentos_ << "\n";

  if (iteraciones > 0) {
    std::cout << "Proporcion media obstaculos: " << suma_obstaculos / iteraciones << "\n";
  }
}