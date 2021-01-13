double main(double pos, double size) {
          if (pos < 5680) {
            return 0.3;
          }else if (pos < 6180) {
            return 0.3 + sqrt((1 - ((6180 - pos) / 500))) * 0.3;
          }
          else if (pos < 6880) {
            return 1;
          }
          else {
            return 1 * sqrt(((8380 - pos) / 1500));
          }
        }