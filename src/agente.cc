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
// Archivo agente.cc: implementación del agente encargado de ejecutar
//                    A* paso a paso en un entorno dinámico.
#include "agente.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>



/**
 * @brief Devuelve el nombre en texto de la heuristica seleccionada.
 *
 * Esta funcion se utiliza para imprimir en pantalla y en el fichero
 * de salida que heuristica se ha empleado durante la ejecucion.
 */
std::string NombreHeuristica(TipoHeuristica tipo) {
  switch (tipo) {
    case TipoHeuristica::MANHATTAN:
      return "Manhattan";
    case TipoHeuristica::EUCLIDEA:
      return "Euclidea";
    case TipoHeuristica::CHEBYSHEV:
      return "Chebyshev";
    case TipoHeuristica::NULA:
      return "Nula";
  }

  return "Desconocida";
}



/**
 * @brief Construye un agente asociado a un laberinto.
 *
 * El agente almacena una referencia al laberinto, su posicion inicial
 * y la posicion objetivo a la que debe llegar.
 */
Agente::Agente(Laberinto& lab, Celda inicio, Celda fin) : lab_(lab), pos_actual_(inicio), objetivo_(fin) {
}



/**
 * @brief Ejecuta la busqueda A* en un entorno dinamico.
 *
 * En cada iteracion se calcula un camino desde la posicion actual hasta
 * el objetivo. El agente avanza una unica casilla por ese camino y, tras
 * el movimiento, el laberinto se actualiza dinamicamente. Si no existe
 * camino, se actualiza el entorno y se reintenta hasta un maximo de cinco
 * veces consecutivas.
 *
 * @param salida Flujo donde se imprime la ejecucion.
 * @param tipo Heuristica empleada por A*.
 * @return ResultadoEjecucion con las metricas finales.
 */
ResultadoEjecucion Agente::Ejecutar(std::ostream& salida,
                                    TipoHeuristica tipo) {
  int pasos = 0;
  int nodos_generados = 0;
  int nodos_inspeccionados = 0;
  int reintentos_totales = 0;

  double suma_obstaculos = 0.0;
  int iteraciones = 0;
  double coste_total = 0.0;

  ResultadoEjecucion resultado;

  salida << "Ejecucion A* en entorno dinamico\n";
  salida << "Heuristica usada: " << NombreHeuristica(tipo) << "\n";
  salida << "Inicio: (" << pos_actual_.x() << ", " << pos_actual_.y() << ")\n";
  salida << "Fin: (" << objetivo_.x() << ", " << objetivo_.y() << ")\n";
  salida << "Limite obstaculos dinamicos: "
         << lab_.MaxObstaculosDinamicos()
         << " de "
         << lab_.Filas() * lab_.Columnas()
         << " casillas (25%)\n\n";

  // Mientras el agente no alcance la meta, se planifica y se avanza paso a paso.
  while (!(pos_actual_.x() == objetivo_.x() &&
           pos_actual_.y() == objetivo_.y())) {
    AStar astar(lab_);
    std::vector<Nodo> camino;

    // Se ejecuta A* sobre el estado actual del laberinto.
    bool encontrado = astar.BuscarCamino(pos_actual_, objetivo_, camino, tipo);

    // Si no se encuentra camino, se actualiza el entorno y se reintenta.
    if (!encontrado) {
      ++reintentos_;
      ++reintentos_totales;

      salida << "Iteracion " << iteraciones << "\n";
      salida << "No existe camino disponible. Reintento "
             << reintentos_ << "/5\n\n";

      // Tras cinco fallos consecutivos, se considera que la meta es inalcanzable.
      if (reintentos_ >= 5) {
        salida << "Destino inalcanzable tras 5 reintentos consecutivos.\n";

        resultado.alcanzado = false;
        resultado.coste = coste_total;
        resultado.pasos = pasos;
        resultado.nodos_generados = nodos_generados;
        resultado.nodos_inspeccionados = nodos_inspeccionados;
        resultado.reintentos = reintentos_totales;
        resultado.obstaculos_medios =
            iteraciones > 0 ? suma_obstaculos / iteraciones : 0.0;

        return resultado;
      }

      lab_.ActualizarDinamico();
      continue;
    }

    // Si se encuentra un camino, se reinicia el contador de fallos consecutivos.
    reintentos_ = 0;

    nodos_generados += astar.GetGenerados();
    nodos_inspeccionados += astar.GetInspeccionados();

    salida << "Iteracion " << iteraciones << "\n";
    salida << "Camino generado:\n";
    lab_.EscribirConCamino(camino, salida);

    salida << "Coste del camino planificado: "
           << astar.GetCosteFinal() << "\n";
    salida << "Nodos generados en esta iteracion: "
           << astar.GetGenerados() << "\n";
    salida << "Nodos inspeccionados en esta iteracion: "
           << astar.GetInspeccionados() << "\n";
    salida << "Obstaculos dinamicos actuales: "
           << lab_.ContarObstaculosDinamicos()
           << " / "
           << lab_.MaxObstaculosDinamicos()
           << "\n";
    salida << "Proporcion obstaculos dinamicos: "
           << lab_.ProporcionObstaculos()
           << "\n\n";

    // El agente solo avanza una casilla. Despues se volvera a planificar.
    if (camino.size() > 1) {
      const bool movimiento_diagonal =
          std::abs(camino[1].x() - camino[0].x()) == 1 &&
          std::abs(camino[1].y() - camino[0].y()) == 1;

      coste_total += movimiento_diagonal ? 7.0 : 5.0;

      pos_actual_ = lab_.GetCelda(camino[1].x(), camino[1].y());
      ++pasos;
    }

    suma_obstaculos += lab_.ProporcionObstaculos();
    ++iteraciones;

    // Tras avanzar, el entorno cambia y la siguiente iteracion replanifica.
    lab_.ActualizarDinamico();
  }

  salida << "\nTabla resumen\n";
  salida << "Heuristica Filas Columnas S E Coste Pasos "
         << "Nodos_generados Nodos_inspeccionados Reintentos "
         << "Obstaculos_medios\n";

  salida << NombreHeuristica(tipo) << " "
         << lab_.Filas() << " "
         << lab_.Columnas() << " "
         << "(" << lab_.BuscarInicio().x() << ","
         << lab_.BuscarInicio().y() << ") "
         << "(" << objetivo_.x() << "," << objetivo_.y() << ") "
         << coste_total << " "
         << pasos << " "
         << nodos_generados << " "
         << nodos_inspeccionados << " "
         << reintentos_totales << " ";

  if (iteraciones > 0) {
    salida << suma_obstaculos / iteraciones << "\n";
  } else {
    salida << "0\n";
  }

  salida << "\nMeta alcanzada.\n";

  resultado.alcanzado = true;
  resultado.coste = coste_total;
  resultado.pasos = pasos;
  resultado.nodos_generados = nodos_generados;
  resultado.nodos_inspeccionados = nodos_inspeccionados;
  resultado.reintentos = reintentos_totales;
  resultado.obstaculos_medios = iteraciones > 0 ? suma_obstaculos / iteraciones : 0.0;

  return resultado;
}