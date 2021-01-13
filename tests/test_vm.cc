// Copyright (c) 2020 smarsufan. All Rights Reserved.

#include <iostream>

#include "api.h"

int main(int argv, char *args[]) {
  if (argv != 2) {
    fprintf(stderr, "Usage: %s <input.sm>", args[0]);
  }

  const char *path = args[1];
  FILE *fb = fopen(path, "r");
  if (!fb) {
    fprintf(stderr, "can not open %s\n", path);
    return -1;
  }

  smcc::ReaderStdio reader(fb);
  smcc::AST ast(&reader);

  ast.parse();
  ast.codegen();

  smcc::VM vm;
  // vm.setArgs(std::vector<double>{1, 2});
  vm.exe("main", std::vector<double>{7000, 8380}, 0);
  fprintf(stderr, "%f = main(15000, 16000)\n", vm.getRet());

  fclose(fb);
}
