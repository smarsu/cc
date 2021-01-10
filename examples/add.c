// double sub(double a, double b, double k) {
//   if (k <= 0) {
//     return a;
//   }

//   return sub(a - 1, b, k - 1);
// }

// double main(double a, double b) {
//   if (a > 2) {
//     a = sub(a, 1, 10);
//   }
//   else if (a > 3) {
//     a = a + 2;
//   }
//   else {
//     a = sub(a, 3, 20);
//   }
//   return a + b;
// }

// double main(double pos, double size) {
//   double list[2] = {0.3, 0.7};  // 支持列表初始化

//   if (pos < list[0] * size) {
//     return sqrt(pos / 0.3 * size);
//   }
//   else if (pos < list[1] * size) {
//     return 1;
//   }
//   else {
//     return sqrt((size - pos) / (0.3 * size));
//   }
// }

double main(double pos, int loop) {
  int i = 0;
  double ret = 0;
  while (i < loop) {
    ret = ret + pos;
    i = i + 1;
  }
  return ret;
}

// double main(double pos, double size) {
//   if (pos < 3730) {
//     return 0.5;
//   }else if (pos < 4730) {
//     return 0.5 + (1 - (4730 - pos / 1000)) * 0.5;
//   }
//   else if (pos < 7960) {
//     return 1;
//   }
//   else {
//     return 1 * (9460 - pos / 1500);
//   }
// }

// double main(double a, double b) {
//   return a + b;
// }
