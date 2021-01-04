double add(double pos, double size) {
  if (pos < 0.3 * size) {
    return 0.5;
  }
  else if (pos < 0.7 * size) {
    return 1;
  }
  else {
    return 0.5;
  }
}
