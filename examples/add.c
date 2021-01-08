double main(double pos, double size) {
  if (pos < 0.3 * size) {
    return sqrt(0.3);
  }
  else if (pos < 0.9 * size) {
    return sin(0.3);
  }
  else {
    return pow((size - pos) / size, 2);
  }
}
