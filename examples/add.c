double add(double x, double y) {
  return x + y;
}

double main(double pos, double size) {
  if (pos < 0.3 * size) {
    return sqrt(add(0.3, 1));
  }
  else if (pos < 0.9 * size) {
    return sin(0.3);
  }
  else {
    return pow((size - pos) / size, 2);
  }
}
