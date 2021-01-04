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

  smcc::reset();
  auto v = smcc::call("add", {8000., 16000.});

  fprintf(stderr, "%f\n", v);  

  fclose(fb);
}
