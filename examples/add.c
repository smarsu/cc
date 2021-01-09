double add(double pos, double size, double times) {
  if (times < 1) {
    return pos;
  }

  return add(pos + size, size, times - 1);
}

double main(double pos, double size) {
  if (pos < 0.3 * size) {
    return sqrt(add(1, 1000, 3));
  }
  else if (pos < 0.9 * size) {
    return sin(0.3);
  }
  else {
    return pow((size - pos) / size, 2);
  }
}
