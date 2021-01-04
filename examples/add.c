double add(double pos, double size) {
  if (pos < 0.3 * size) {
    return 0.3;
  }
  else if (pos < 0.9 * size) {
    return 1;
  }
  else {
    return (size - pos) / size;
  }
}
