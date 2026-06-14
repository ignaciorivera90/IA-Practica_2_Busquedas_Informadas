#include "funciones.h"

#include "laberinto.h"
#include "agente.h"

int main(int argc, char* argv[]) {

  if (argc == 2 &&
      std::string(argv[1]) == "--help") {
    help();
    return 0;
  }

  if (argc != 3) {
    help();
    return 1;
  }

  std::string in_file = argv[1];

  try {
    Laberinto lab(in_file);
    Celda inicio = lab.BuscarInicio();
    Celda fin = lab.BuscarFin();
    Agente agente(lab, inicio, fin);
    agente.Ejecutar();

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}