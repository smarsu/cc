// Copyright (c) 2020 smarsufan. All Rights Reserved.

#pragma once
#include <string>
// #include "ir.h"
namespace smcc {

class CodeGen {
 public:
  CodeGen() {}

  ~CodeGen() {}
};

class VM {
 public:
  VM() {}

  ~VM() {}

  // void setArgs(const std::vector<double> &args);

  double getRet();

  void exe(const std::string &func, const std::vector<double> &args, int this_stack_index_ = 0);

 private:
  // void *builder_{nullptr};
  // int this_stack_index_{0};
};

}  // namespace smcc
