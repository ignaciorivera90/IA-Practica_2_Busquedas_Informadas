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
// Archivo laberinto.cc: implementación de la clase Laberinto,
//                       lectura del mapa, actualización dinámica
//                       y representación del camino.

#include "laberinto.h"

#include <stdexcept>
#include <random>
#include <algorithm>
#include <set>



/**
 * @brief Construye un laberinto a partir de un fichero.
 *
 * Llama al método Cargar para leer la instancia indicada.
 */
Laberinto::Laberinto(const std::string& nombre_archivo) {
  Cargar(nombre_archivo);
}



/**
 * @brief Lee el laberinto desde un fichero de texto.
 *
 * El fichero debe contener primero el número de filas, luego el número
 * de columnas y finalmente la matriz del laberinto. Además, se guardan
 * las paredes originales para que no sean modificadas por el entorno
 * dinámico.
 */
void Laberinto::Cargar(const std::string& nombre_archivo) {
  std::ifstream archivo(nombre_archivo);

  if (!archivo) {
    throw std::runtime_error("No se pudo abrir el archivo: " + nombre_archivo);
  }

  archivo >> filas_;
  archivo >> columnas_;

  grid_.clear();
  grid_.resize(filas_, std::vector<Celda>(columnas_));

  paredes_originales_.clear();
  paredes_originales_.resize(filas_, std::vector<bool>(columnas_, false));

  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      int tipo;
      archivo >> tipo;

      grid_[i][j] = Celda(i, j, tipo);
			if (tipo == 1) {
				paredes_originales_[i][j] = true;
			}
    }
  }
}



/**
 * @brief Muestra el laberinto actual por pantalla.
 */
void Laberinto::Mostrar() const {
  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      std::cout << grid_[i][j].tipo() << " ";
    }
    std::cout << '\n';
  }
}



/**
 * @brief Muestra el laberinto por pantalla marcando el camino con '*'.
 */
void Laberinto::MostrarConCamino(const std::vector<Nodo>& camino) const {
  EscribirConCamino(camino, std::cout);
}



/**
 * @brief Escribe el laberinto en un flujo marcando el camino con '*'.
 *
 * Las celdas de inicio y fin mantienen su valor original, mientras que
 * las celdas intermedias del camino se muestran con un asterisco.
 */
void Laberinto::EscribirConCamino(const std::vector<Nodo>& camino, std::ostream& os) const {
  std::set<std::pair<int, int>> posiciones_camino;

  for (const Nodo& nodo : camino) {
    posiciones_camino.insert({nodo.x(), nodo.y()});
  }

  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      int tipo = grid_[i][j].tipo();

      if (posiciones_camino.count({i, j}) && tipo != 3 && tipo != 4) {
        os << "* ";
      } else {
        os << tipo << " ";
      }
    }
    os << '\n';
  }
}



/**
 * @brief Comprueba si una celda puede ser recorrida.
 *
 * Una celda es transitable si está dentro de los límites del laberinto
 * y no es un obstáculo.
 */
bool Laberinto::EsTransitable(int x, int y) const {
  if (x < 0 || x >= filas_ || y < 0 || y >= columnas_) {
    return false;
  }

  return grid_[x][y].EsLibre();
}



/**
 * @brief Actualiza los obstáculos dinámicos del laberinto.
 *
 * Las paredes originales, la entrada y la salida no cambian. El resto de
 * celdas pueden pasar de libres a obstáculos o de obstáculos dinámicos a
 * libres según las probabilidades pin_ y pout_.
 */
void Laberinto::ActualizarDinamico() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      int tipo = grid_[i][j].tipo();

      // No se modifican la entrada, la salida ni las paredes originales.
      if (tipo == 3 || tipo == 4 || EsParedOriginal(i, j)) {
        continue;
      }

      double u = dist(gen);

      if (tipo == 0) {
        // Libre -> obstaculo dinamico con probabilidad pin_
        if (u >= (1.0 - pin_)) {
          grid_[i][j].setTipo(1);
        }
      } else if (tipo == 1) {
        // Obstaculo dinamico -> libre con probabilidad pout_
        if (u >= (1.0 - pout_)) {
          grid_[i][j].setTipo(0);
        }
      }
    }
  }

  LimitarObstaculosDinamicos();
}



/**
 * @brief Busca la celda marcada como inicio.
 *
 * @return Celda que contiene el valor 3.
 */
Celda Laberinto::BuscarInicio() const {
  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      if (grid_[i][j].tipo() == 3) {
        return grid_[i][j];
      }
    }
  }

  throw std::runtime_error("No se encontro la celda de inicio 3");
}



/**
 * @brief Busca la celda marcada como salida.
 *
 * @return Celda que contiene el valor 4.
 */
Celda Laberinto::BuscarFin() const {
  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      if (grid_[i][j].tipo() == 4) {
        return grid_[i][j];
      }
    }
  }

  throw std::runtime_error("No se encontro la celda final 4");
}



/**
 * @brief Calcula la proporción de obstáculos dinámicos.
 *
 * No se tienen en cuenta las paredes originales, solo los obstáculos que
 * aparecen durante la ejecución dinámica.
 */
double Laberinto::ProporcionObstaculos() const {
  int obstaculos_dinamicos = 0;
  int total = filas_ * columnas_;

  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      if (grid_[i][j].tipo() == 1 && !EsParedOriginal(i, j)) {
        ++obstaculos_dinamicos;
      }
    }
  }

  return static_cast<double>(obstaculos_dinamicos) / total;
}



/**
 * @brief Comprueba si una celda era pared en el laberinto original.
 */
bool Laberinto::EsParedOriginal(int x, int y) const {
  if (x < 0 || x >= filas_ || y < 0 || y >= columnas_) {
    return false;
  }

  return paredes_originales_[x][y];
}



/**
 * @brief Limita los obstáculos dinámicos al 25% del total de casillas.
 *
 * Si se supera el máximo permitido, se seleccionan obstáculos dinámicos
 * aleatoriamente y se convierten en casillas libres hasta cumplir el límite.
 */
void Laberinto::LimitarObstaculosDinamicos() {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  const int total_celdas = filas_ * columnas_;
  const int max_obstaculos_dinamicos =
      static_cast<int>(total_celdas * 0.25);

  std::vector<std::pair<int, int>> obstaculos_dinamicos;

  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      if (grid_[i][j].tipo() == 1 && !EsParedOriginal(i, j)) {
        obstaculos_dinamicos.push_back({i, j});
      }
    }
  }

  if (static_cast<int>(obstaculos_dinamicos.size()) <=
      max_obstaculos_dinamicos) {
    return;
  }

  std::shuffle(obstaculos_dinamicos.begin(),
               obstaculos_dinamicos.end(),
               gen);

  int exceso =
      static_cast<int>(obstaculos_dinamicos.size()) -
      max_obstaculos_dinamicos;

  for (int k = 0; k < exceso; ++k) {
    int x = obstaculos_dinamicos[k].first;
    int y = obstaculos_dinamicos[k].second;

    grid_[x][y].setTipo(0);
  }
}



/**
 * @brief Cuenta cuántos obstáculos dinámicos hay actualmente.
 */
int Laberinto::ContarObstaculosDinamicos() const {
  int contador = 0;

  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      if (grid_[i][j].tipo() == 1 && !EsParedOriginal(i, j)) {
        ++contador;
      }
    }
  }

  return contador;
}



/**
 * @brief Devuelve el máximo de obstáculos dinámicos permitidos.
 */
int Laberinto::MaxObstaculosDinamicos() const {
  return static_cast<int>((filas_ * columnas_) * 0.25);
}



/**
 * @brief Convierte una celda en pared original fija.
 *
 * Se usa cuando el usuario cambia manualmente las posiciones de inicio
 * y salida, haciendo que las antiguas posiciones pasen a ser paredes.
 */
void Laberinto::MarcarParedOriginal(int x, int y) {
  if (x >= 0 && x < filas_ && y >= 0 && y < columnas_) {
    paredes_originales_[x][y] = true;
    grid_[x][y].setTipo(1);
  }
}