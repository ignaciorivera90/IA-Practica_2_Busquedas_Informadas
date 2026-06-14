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
// Archivo main.cc: programa principal encargado de leer argumentos,
//                  configurar la ejecución y lanzar el agente A*.

#include "funciones.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "laberinto.h"
#include "agente.h"
#include "AStar.h"



/**
 * @brief Punto de entrada del programa.
 *
 * Lee el fichero de entrada y el fichero de salida desde la línea de
 * comandos. Después permite seleccionar las posiciones de inicio y salida,
 * la heurística y el número de ejecuciones. Cada ejecución se realiza sobre
 * una copia nueva del laberinto original para que las pruebas sean
 * independientes.
 */
int main(int argc, char* argv[]) {
  if (argc == 2 && std::string(argv[1]) == "--help") {
    help();
    return 0;
  }

  if (argc != 3) {
    help();
    return 1;
  }

  std::string in_file = argv[1];
  std::string out_file = argv[2];

  try {
    // Se carga un laberinto base para validar el fichero y obtener S y E.
    Laberinto lab_base(in_file);

    Celda inicio = lab_base.BuscarInicio();
    Celda fin = lab_base.BuscarFin();

    char opcion_coordenadas;
    std::cout << "Quieres introducir S y E por teclado? (s/n): ";
    std::cin >> opcion_coordenadas;

    // Permite sustituir las coordenadas de entrada y salida del fichero.
    if (opcion_coordenadas == 's' || opcion_coordenadas == 'S') {
      int sx, sy, ex, ey;

      std::cout << "Introduce fila y columna de inicio: ";
      std::cin >> sx >> sy;

      std::cout << "Introduce fila y columna de salida: ";
      std::cin >> ex >> ey;

      if (!lab_base.EsTransitable(sx, sy) || !lab_base.EsTransitable(ex, ey)) {
        std::cerr << "Error: inicio o salida no transitables.\n";
        return 1;
      }

      inicio = lab_base.GetCelda(sx, sy);
      fin = lab_base.GetCelda(ex, ey);
    }

    int opcion_heuristica;
    std::cout << "\nSelecciona heuristica:\n";
    std::cout << "1. Manhattan\n";
    std::cout << "2. Euclidea\n";
    std::cout << "3. Chebyshev\n";
    std::cout << "4. Nula\n";
    std::cout << "Opcion: ";
    std::cin >> opcion_heuristica;

    TipoHeuristica heuristica = TipoHeuristica::MANHATTAN;

    // Conversión de la opción numérica al tipo de heurística correspondiente.
    if (opcion_heuristica == 2) {
      heuristica = TipoHeuristica::EUCLIDEA;
    } else if (opcion_heuristica == 3) {
      heuristica = TipoHeuristica::CHEBYSHEV;
    } else if (opcion_heuristica == 4) {
      heuristica = TipoHeuristica::NULA;
    }

    int numero_ejecuciones = 1;
    std::cout << "Numero de ejecuciones: ";
    std::cin >> numero_ejecuciones;

    if (numero_ejecuciones <= 0) {
      std::cerr << "Error: el numero de ejecuciones debe ser mayor que 0.\n";
      return 1;
    }

    std::ofstream salida(out_file);

    if (!salida) {
      std::cerr << "Error: no se pudo abrir el fichero de salida.\n";
      return 1;
    }

    std::ostringstream buffer_salida;

    int alcanzadas = 0;
    int fallidas = 0;

    double suma_coste = 0.0;
    double suma_pasos = 0.0;
    double suma_generados = 0.0;
    double suma_inspeccionados = 0.0;
    double suma_reintentos = 0.0;
    double suma_obstaculos = 0.0;

    // Ejecuta el experimento tantas veces como indique el usuario.
    for (int i = 1; i <= numero_ejecuciones; ++i) {
      buffer_salida << "\n========================================\n";
      buffer_salida << "EJECUCION " << i << "\n";
      buffer_salida << "========================================\n\n";

      // Cada ejecución empieza desde el laberinto original.
      Laberinto lab_ejecucion(in_file);

      Celda inicio_ejecucion = lab_ejecucion.BuscarInicio();
      Celda fin_ejecucion = lab_ejecucion.BuscarFin();

      // Si se introdujeron coordenadas manuales, se aplican a la copia actual.
      if (opcion_coordenadas == 's' || opcion_coordenadas == 'S') {
        lab_ejecucion.MarcarParedOriginal(inicio_ejecucion.x(), inicio_ejecucion.y());
        lab_ejecucion.MarcarParedOriginal(fin_ejecucion.x(), fin_ejecucion.y());

        lab_ejecucion.GetCelda(inicio.x(), inicio.y()).setTipo(3);
        lab_ejecucion.GetCelda(fin.x(), fin.y()).setTipo(4);

        inicio_ejecucion = lab_ejecucion.GetCelda(inicio.x(), inicio.y());
        fin_ejecucion = lab_ejecucion.GetCelda(fin.x(), fin.y());
      }

      Agente agente(lab_ejecucion, inicio_ejecucion, fin_ejecucion);

      ResultadoEjecucion resultado =
          agente.Ejecutar(buffer_salida, heuristica);

      if (resultado.alcanzado) {
        ++alcanzadas;
      } else {
        ++fallidas;
      }

      suma_coste += resultado.coste;
      suma_pasos += resultado.pasos;
      suma_generados += resultado.nodos_generados;
      suma_inspeccionados += resultado.nodos_inspeccionados;
      suma_reintentos += resultado.reintentos;
      suma_obstaculos += resultado.obstaculos_medios;
    }

    // Resumen estadístico de todas las ejecuciones realizadas.
    buffer_salida << "\n========================================\n";
    buffer_salida << "RESUMEN GLOBAL\n";
    buffer_salida << "========================================\n";
    buffer_salida << "Ejecuciones totales: " << numero_ejecuciones << "\n";
    buffer_salida << "Ejecuciones alcanzadas: " << alcanzadas << "\n";
    buffer_salida << "Ejecuciones fallidas: " << fallidas << "\n";

    buffer_salida << "Coste medio: " << suma_coste / numero_ejecuciones << "\n";
    buffer_salida << "Pasos medios: " << suma_pasos / numero_ejecuciones << "\n";
    buffer_salida << "Nodos generados medios: " << suma_generados / numero_ejecuciones << "\n";
    buffer_salida << "Nodos inspeccionados medios: " << suma_inspeccionados / numero_ejecuciones << "\n";
    buffer_salida << "Reintentos medios: " << suma_reintentos / numero_ejecuciones << "\n";
    buffer_salida << "Obstaculos medios: " << suma_obstaculos / numero_ejecuciones << "\n";

    // La salida se muestra por pantalla y también se vuelca al fichero.
    std::cout << buffer_salida.str();
    salida << buffer_salida.str();

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}