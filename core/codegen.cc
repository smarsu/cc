// Copyright (c) 2020 smarsufan. All Rights Reserved.

#include <cstring>

#include "codegen.h"
#include "ir.h"
#include "expr.h"

namespace smcc {

double VM::getRet() {
  return GlobalStack::Global()->stack()[0];
}

void VM::exe(const std::string &func, const std::vector<double> &args, int this_stack_index) {
  double *stack = GlobalStack::Global()->stack();
  memcpy(stack + this_stack_index + 1, args.data(), sizeof(double) * args.size());
  // builder_->run(func, values);
  ir_builder()->fuction(func)->run(this, this_stack_index);
}

}  // namespace smcc
