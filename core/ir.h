// Copyright (c) 2020 smarsufan. All Rights Reserved.

#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <cmath>

#include "codegen.h"

namespace smcc {

enum Area {
  kGlobalConst,
  kGlobalStack,
};

class GlobalArea {
 public:
  static GlobalArea *Global() {
    static GlobalArea gconsts;
    return &gconsts;
  }

 protected:
  GlobalArea() {}

  virtual ~GlobalArea() {}

 protected:
  std::vector<double> consts;
  std::map<double, int> consts_map;
};

class GlobalConst : public GlobalArea {
 public:
  static GlobalConst *Global() {
    static GlobalConst gconsts;
    return &gconsts;
  }

  int add(double value) {
    if (consts_map.find(value) != consts_map.end()) {
      return consts_map[value];
    }

    consts.push_back(value);
    int index = consts.size() - 1;
    consts_map[value] = index;
    return index;
  }

  double value(int index) {
    return consts[index];
  }

  double *valuep(int index) {
    return consts.data() + index;
  }

 private:
  GlobalConst() {}

  ~GlobalConst() {}
};

class GlobalStack : public GlobalArea {
 public:
  static GlobalStack *Global() {
    static GlobalStack gconsts;
    return &gconsts;
  }

  double *stack() { return consts.data(); }

 private:
  GlobalStack() { consts.resize(8 * 1024); }

  ~GlobalStack() {}
};

class IRValue {
 public:
  IRValue(int id, Area area=kGlobalStack);

  ~IRValue();

  int id() { return id_; }

  double *value(int this_stack_index) { 
    switch (area_) {
      case kGlobalStack:
        return GlobalStack::Global()->stack() + id_ + this_stack_index; 
      
      case kGlobalConst:
        return GlobalConst::Global()->valuep(id_);

      default:
        abort();
    }
  }

  std::string toString();

 private:
  int id_;
  Area area_;
};

class IROp {
 public:
  IROp(int op, std::vector<IRValue *> values);

  std::vector<IRValue *> &values() { return values_; }

  virtual ~IROp();

  virtual int run(VM *builder, int this_stack_index) = 0;

  virtual std::string toString();

 protected:
  int op_;
  std::vector<IRValue *> values_;
};

class IRCallOp : public IROp {
 public:
  IRCallOp(std::string name, int op, std::vector<IRValue *> values) : IROp(op, values), name_(name) {
  }

  ~IRCallOp() = default;

  std::string name() { return name_; }

  virtual int run(VM *vm, int this_stack_index) {
    if (name_ == "sqrt") {
      values_[0]->value(this_stack_index)[0] = std::sqrt(values_[1]->value(this_stack_index)[0]);
    }
    else if (name_ == "sin") {
      values_[0]->value(this_stack_index)[0] = std::sin(values_[1]->value(this_stack_index)[0]);
    }
    else if (name_ == "pow") {
      values_[0]->value(this_stack_index)[0] = std::pow(values_[1]->value(this_stack_index)[0], values_[2]->value(this_stack_index)[0]);
    }
    else {
      std::vector<double> args(values_.size() - 1);
      for (int i = 1; i < values_.size(); ++i) {
        args[i - 1] = values_[i]->value(this_stack_index)[0];
      }
      vm->exe(name_, args, this_stack_index + values_[0]->id());
    }
    return -1;
  }

  virtual std::string toString();

 private:
  std::string name_;
};

class IRIfOp : public IROp {
 public:
  IRIfOp(int block_if, int block_else, int op, std::vector<IRValue *> values) : IROp(op, values), block_if_(block_if), block_else_(block_else) {
  }

  ~IRIfOp() = default;

  virtual int run(VM *vm, int this_stack_index) {
    if (values_[0]->value(this_stack_index)[0]) {
      return block_if_;
    }
    else {
      return block_else_;
    }
  }

  virtual std::string toString();

 private:
  int block_if_;
  int block_else_;
};

class IRGotoOp : public IROp {
 public:
  IRGotoOp(int block_goto, int op, std::vector<IRValue *> values) : IROp(op, values), block_goto_(block_goto) {
  }

  ~IRGotoOp() = default;

  virtual int run(VM *builder, int this_stack_index) {
    return block_goto_;
  }

  virtual std::string toString();

 private:
  int block_goto_;
};

class IRLessOp : public IROp {
 public:
  IRLessOp(int op, std::vector<IRValue *> values) : IROp(op, values) {
  }

  ~IRLessOp() = default;

  virtual int run(VM *vm, int this_stack_index) {
    values_[0]->value(this_stack_index)[0] = values_[1]->value(this_stack_index)[0] < values_[2]->value(this_stack_index)[0];
    return -1;
  }

  virtual std::string toString();
};

class IRLessEqualOp : public IROp {
 public:
  IRLessEqualOp(int op, std::vector<IRValue *> values) : IROp(op, values) {
  }

  ~IRLessEqualOp() = default;

  virtual int run(VM *vm, int this_stack_index) {
    values_[0]->value(this_stack_index)[0] = values_[1]->value(this_stack_index)[0] <= values_[2]->value(this_stack_index)[0];
    return -1;
  }

  virtual std::string toString();
};

class IRGreatOp : public IROp {
 public:
  IRGreatOp(int op, std::vector<IRValue *> values) : IROp(op, values) {
  }

  ~IRGreatOp() = default;

  virtual int run(VM *vm, int this_stack_index) {
    values_[0]->value(this_stack_index)[0] = values_[1]->value(this_stack_index)[0] > values_[2]->value(this_stack_index)[0];
    return -1;
  }

  virtual std::string toString();
};

class IRGreatEqualOp : public IROp {
 public:
  IRGreatEqualOp(int op, std::vector<IRValue *> values) : IROp(op, values) {
  }

  ~IRGreatEqualOp() = default;

  virtual int run(VM *vm, int this_stack_index) {
    values_[0]->value(this_stack_index)[0] = values_[1]->value(this_stack_index)[0] >= values_[2]->value(this_stack_index)[0];
    return -1;
  }

  virtual std::string toString();
};

class IREqualOp : public IROp {
 public:
  IREqualOp(int op, std::vector<IRValue *> values) : IROp(op, values) {
  }

  ~IREqualOp() = default;

  virtual int run(VM *vm, int this_stack_index) {
    values_[0]->value(this_stack_index)[0] = values_[1]->value(this_stack_index)[0] == values_[2]->value(this_stack_index)[0];
    return -1;
  }

  virtual std::string toString();
};

class IRAddOp : public IROp {
 public:
  IRAddOp(int op, std::vector<IRValue *> values) : IROp(op, values) {
  }

  ~IRAddOp() = default;

  virtual int run(VM *vm, int this_stack_index) {
    values_[0]->value(this_stack_index)[0] = values_[1]->value(this_stack_index)[0] + values_[2]->value(this_stack_index)[0];
    return -1;
  }

  virtual std::string toString();
};

class IRSubOp : public IROp {
 public:
  IRSubOp(int op, std::vector<IRValue *> values) : IROp(op, values) {
  }

  ~IRSubOp() = default;

  virtual int run(VM *vm, int this_stack_index) {
    values_[0]->value(this_stack_index)[0] = values_[1]->value(this_stack_index)[0] - values_[2]->value(this_stack_index)[0];
    return -1;
  }

  virtual std::string toString();
};

class IRMulOp : public IROp {
 public:
  IRMulOp(int op, std::vector<IRValue *> values) : IROp(op, values) {
  }

  ~IRMulOp() = default;

  virtual int run(VM *vm, int this_stack_index) {
    values_[0]->value(this_stack_index)[0] = values_[1]->value(this_stack_index)[0] * values_[2]->value(this_stack_index)[0];
    return -1;
  }

  virtual std::string toString();
};

class IRDivOp : public IROp {
 public:
  IRDivOp(int op, std::vector<IRValue *> values) : IROp(op, values) {
  }

  ~IRDivOp() = default;

  virtual int run(VM *vm, int this_stack_index) {
    values_[0]->value(this_stack_index)[0] = values_[1]->value(this_stack_index)[0] / values_[2]->value(this_stack_index)[0];
    return -1;
  }

  virtual std::string toString();
};

class IRAssignOp : public IROp {
 public:
  IRAssignOp(int op, std::vector<IRValue *> values) : IROp(op, values) {
  }

  ~IRAssignOp() = default;

  virtual int run(VM *vm, int this_stack_index) {
    values_[0]->value(this_stack_index)[0] = values_[1]->value(this_stack_index)[0];
    return -1;
  }

  virtual std::string toString();
};

class IRRetOp : public IROp {
 public:
  IRRetOp(int op, std::vector<IRValue *> values) : IROp(op, values) {}

  ~IRRetOp() = default;

  virtual int run(VM *vm, int this_stack_index) {
    GlobalStack::Global()->stack()[this_stack_index] = values_[0]->value(this_stack_index)[0];
    return -2;  // -2 means return
  }

  virtual std::string toString();
};

class IRFunction {
 public:
  IRFunction(std::string name, std::vector<std::string> args);

  ~IRFunction() = default;

  void run(VM *vm, int this_stack_index);

  int id() { return this_id; }

  int op_size() { return ops.size(); }

  std::vector<std::unique_ptr<IROp>> &ir_ops() { return ops; }

  void setId(int id);

  void setGoto(int block_id, int pos) {
    goto_map[block_id] = pos;
  }

  IRValue *CreateNumber(double value);

  IRValue *CreateValue(std::string name, bool isNew);

  IRValue *CreateBinary(IRValue *left, IRValue *right, int op);

  IRValue *CreateIf(IRValue *cond, int *block_if, int *block_else);

  IRValue *CreateGoto(int *block_end);

  IRValue *CreateCall(std::string name, std::vector<IRValue *> args);

  IRValue *CreateRet(IRValue *value);

  std::string toString();

 private:
  void addValue(std::unique_ptr<IRValue> value, std::string name) {
    name_to_value[name] = value.get();
    values.push_back(std::move(value)); 
  }

  void addOp(std::unique_ptr<IROp> op) { ops.push_back(std::move(op)); }

 private:
  std::string name_; 
  std::vector<std::string> args_;
  std::vector<std::unique_ptr<IROp>> ops;
  std::vector<std::unique_ptr<IRValue>> values;
  std::map<int, int> goto_map;
  std::map<std::string, IRValue *> name_to_value;  // Need to restore when goto end of body.
  int this_id{0};
  int this_block{0};

  int this_stack_index_{-1};
};

class IRBuilder {
 public:
  IRBuilder();

  ~IRBuilder();

  void addFunction(std::string name, std::vector<std::string> args);

  IRFunction *fuction() { return ir_functions_.back().get(); }

  IRFunction *fuction(const std::string name) { return ir_function_map_[name]; }

  int id() { return ir_functions_.back().get()->id(); }

  void setId(int id) { ir_functions_.back().get()->setId(id); }

  // void run(const std::string &func, const std::vector<double> &values);

  std::string toString();

 private:
  // std::vector<std::unique_ptr<Expr>> exprs_;
  std::vector<std::unique_ptr<IRFunction>> ir_functions_;
  std::map<std::string, IRFunction *> ir_function_map_;

  // int this_stack_index{0};
};

}  // namespace smcc
