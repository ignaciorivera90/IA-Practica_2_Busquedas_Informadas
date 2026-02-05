#ifndef CELDA_H
#define CELDA_H

class Celda {
 public:
  Celda(int x = 0, int y = 0, int tipo = 0) : x_(x), y_(y), tipo_(tipo) {}

  int x() const { return x_; }
  int y() const { return y_; }
  int tipo() const { return tipo_; }  // 0 libre, 1 obstáculo, 3 inicio, 4 fin
  void setTipo(int t) { tipo_ = t; }

  bool EsLibre() const { return tipo_ != 1; }

 private:
  int x_;
  int y_;
  int tipo_;
};

#endif