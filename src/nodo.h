#ifndef NODO_H
#define NODO_H

class Nodo {
 public:
  Nodo(int x = 0, int y = 0, double g = 0.0, double h = 0.0)
      : x_(x), y_(y), g_(g), h_(h) {}

  int x() const { return x_; }
  int y() const { return y_; }

  double g() const { return g_; }
  double h() const { return h_; }
  double f() const { return g_ + h_; }

  void setG(double g) { g_ = g; }
  void setH(double h) { h_ = h; }

  bool operator==(const Nodo& other) const {
    return x_ == other.x_ && y_ == other.y_;
  }

 private:
  int x_;
  int y_;
  double g_;
  double h_;
};

#endif