#include "laberinto.h"

#include <stdexcept>
#include <random>
#include <algorithm>
#include <set>

Laberinto::Laberinto(const std::string& nombre_archivo) {
  Cargar(nombre_archivo);
}

void Laberinto::Cargar(const std::string& nombre_archivo) {
  std::ifstream archivo(nombre_archivo);

  if (!archivo) {
    throw std::runtime_error("No se pudo abrir el archivo: " + nombre_archivo);
  }

  archivo >> filas_;
  archivo >> columnas_;

  grid_.clear();
  grid_.resize(filas_, std::vector<Celda>(columnas_));

  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      int tipo;
      archivo >> tipo;
      grid_[i][j] = Celda(i, j, tipo);
    }
  }
}

void Laberinto::Mostrar() const {
  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      std::cout << grid_[i][j].tipo() << " ";
    }
    std::cout << '\n';
  }
}

void Laberinto::MostrarConCamino(const std::vector<Nodo>& camino) const {
  EscribirConCamino(camino, std::cout);
}

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

bool Laberinto::EsTransitable(int x, int y) const {
  if (x < 0 || x >= filas_ || y < 0 || y >= columnas_) {
    return false;
  }

  return grid_[x][y].EsLibre();
}

void Laberinto::ActualizarDinamico() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  const int total_celdas = filas_ * columnas_;
  const int max_obstaculos = static_cast<int>(total_celdas * 0.25);

  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      int tipo = grid_[i][j].tipo();

      if (tipo == 3 || tipo == 4) {
        continue;
      }

      double u = dist(gen);

      if (tipo == 0) {
        if (u >= (1.0 - pin_)) {
          grid_[i][j].setTipo(1);
        }
      } else if (tipo == 1) {
        if (u >= (1.0 - pout_)) {
          grid_[i][j].setTipo(0);
        }
      }
    }
  }

  std::vector<std::pair<int, int>> obstaculos;

  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      if (grid_[i][j].tipo() == 1) {
        obstaculos.push_back({i, j});
      }
    }
  }

  if (static_cast<int>(obstaculos.size()) > max_obstaculos) {
    std::shuffle(obstaculos.begin(), obstaculos.end(), gen);
    int exceso = static_cast<int>(obstaculos.size()) - max_obstaculos;

    for (int k = 0; k < exceso; ++k) {
      grid_[obstaculos[k].first][obstaculos[k].second].setTipo(0);
    }
  }
}

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

double Laberinto::ProporcionObstaculos() const {
  int obstaculos = 0;
  int total = filas_ * columnas_;

  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      if (grid_[i][j].tipo() == 1) {
        ++obstaculos;
      }
    }
  }

  return static_cast<double>(obstaculos) / total;
}