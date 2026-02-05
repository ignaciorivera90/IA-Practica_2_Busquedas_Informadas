#ifndef NODO_H
#define NODO_H

class Nodo {
 public:
  Nodo(int x, int y, double g = 0.0, double h = 0.0, Nodo* padre = nullptr)
      : x_(x), y_(y), g_(g), h_(h), padre_(padre) {}

  double f() const { return g_ + h_; }
  double g() const { return g_; }
  double h() const { return h_; }
  void setG(double g) { g_ = g; }
  void setH(double h) { h_ = h; }
  void setPadre(Nodo* p) { padre_ = p; }
  Nodo* padre() const { return padre_; }

  int x() const { return x_; }
  int y() const { return y_; }

  bool operator==(const Nodo& other) const { return x_ == other.x_ && y_ == other.y_; }

 private:
  int x_;
  int y_;
  double g_;
  double h_;
  Nodo* padre_;
};

#endif
