#ifndef AGENTE_H
#define AGENTE_H

#include "laberinto.h"
#include "celda.h"

class Agente {
 public:
  Agente(Laberinto& lab, Celda inicio, Celda fin);
  void Ejecutar();

 private:
  Laberinto& lab_;
  Celda pos_actual_;
  Celda objetivo_;
  int reintentos_ = 0;
};

#endif