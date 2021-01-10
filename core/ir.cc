// Copyright (c) 2020 smarsufan. All Rights Reserved.

#include <sstream>

#include "ast.h"
#include "ir.h"

namespace smcc {

IRValue::IRValue(int id, Area area)
    : id_(id), area_(area) {
}

IRValue::~IRValue() {
}

std::string IRValue::toString() { 
  switch (area_) {
    default:
      abort();

    case kGlobalStack:
      return "%" + std::to_string(id_);

    case  kGlobalConst:
      return std::to_string(GlobalConst::Global()->value(id_));
  }
}

IRBuilder::IRBuilder() {
}

IRBuilder::~IRBuilder() {
}

// void IRBuilder::run(const std::string &func_name, const std::vector<double> &values) {
//   int stack_index = this_stack_index;
//   auto &func = ir_function_map_[func_name];
//   func->run();
// }

std::string IRBuilder::toString() {
  std::stringstream ss;
  for (auto &func : ir_functions_) {
    ss << func->toString();
    ss << "\n\n";
  }
  return ss.str();
}

void IRBuilder::addFunction(std::string name, std::vector<std::string> args) {
  std::unique_ptr<IRFunction> ir_function = std::make_unique<IRFunction>(name, args);
    ir_function_map_[name] = ir_function.get();
  ir_functions_.push_back(std::move(ir_function));
}

IRFunction::IRFunction(std::string name, std::vector<std::string> args) 
    : name_(std::move(name)), args_(std::move(args)) {
  for (auto &arg : args_) {
    std::unique_ptr<IRValue> ir_value = std::make_unique<IRValue>(++this_id);
    addValue(std::move(ir_value), arg);
  }
}

void IRFunction::run(VM *vm, int this_stack_index) {
  this_stack_index_ = this_stack_index;
  int op_index = 0;
  while (op_index < ops.size()) {
    auto &op = ops[op_index];
    int block = op->run(vm, this_stack_index);
    if (block == -2) {
      // return
      break;
    }
    else if (block == -1) {
      ++op_index;
    }
    else {
      op_index = goto_map[block];
    }
  }
}

void IRFunction::setId(int id) {
  this_id = id;
  for (auto it = name_to_value.begin(); it != name_to_value.end(); ) {
    if (it->second->id() > id) {
      it = name_to_value.erase(it);
    }
    else {
      ++it;
    }
  }
}

std::string IRFunction::toString() {
  std::stringstream ss;
  ss << "@" << name_ << "(";
  for (auto &arg : args_) {
    ss << name_to_value[arg]->toString() << ", ";
  }
  ss << ")";
  ss << "\n";
  for (int idx = 0; idx < ops.size(); ++idx) {
    for (auto &v : goto_map) {
      if (v.second == idx) {
        ss << "L" << std::to_string(v.first) << ":\n";
        // break;
      }
    }
    auto &op = ops[idx];
    ss << op->toString();
  }
  return ss.str();
}

IRValue *IRFunction::CreateNumber(double value) {
  GlobalConst *gconst = GlobalConst::Global();
  int index = gconst->add(value);
  std::unique_ptr<IRValue> ir_value = std::make_unique<IRValue>(index, kGlobalConst);
  IRValue *ir_value_ = ir_value.get();
  addValue(std::move(ir_value), std::to_string(value));
  return ir_value_;
}

IRValue *IRFunction::CreateValue(std::string name, bool isNew) {
  if (isNew) {
    if (name_to_value.find(name) != name_to_value.end()) {
      fprintf(stderr, "ir -4000\n");
      abort();
    }

    std::unique_ptr<IRValue> ir_value = std::make_unique<IRValue>(++this_id);
    IRValue *ir_value_ = ir_value.get();
    addValue(std::move(ir_value), name);
    return ir_value_;
  }
  else {
    if (name_to_value.find(name) == name_to_value.end()) {
      fprintf(stderr, "ir -2000\n");
      abort();
    }

    return name_to_value[name];
  }
}

IRValue *IRFunction::CreateBinary(IRValue *left, IRValue *right, int op) {
  std::unique_ptr<IRValue> ir_value = std::make_unique<IRValue>(++this_id);
  IRValue *ir_value_ = ir_value.get();
  std::unique_ptr<IROp> ir_op;

  switch (op) {
    default:
      fprintf(stderr, "ir -3000\n");
      abort();

    case tok_less:
      ir_op = std::make_unique<IRLessOp>(op, std::vector<IRValue *>{ir_value.get(), left, right});
      break;

    case tok_lessequal:
      ir_op = std::make_unique<IRLessEqualOp>(op, std::vector<IRValue *>{ir_value.get(), left, right});
      break;

    case tok_great:
      ir_op = std::make_unique<IRGreatOp>(op, std::vector<IRValue *>{ir_value.get(), left, right});
      break;

    case tok_greatequal:
      ir_op = std::make_unique<IRGreatEqualOp>(op, std::vector<IRValue *>{ir_value.get(), left, right});
      break;

    case tok_equal:
      ir_op = std::make_unique<IREqualOp>(op, std::vector<IRValue *>{ir_value.get(), left, right});
      break;

    case tok_add:
      ir_op = std::make_unique<IRAddOp>(op, std::vector<IRValue *>{ir_value.get(), left, right});
      break;

    case tok_sub:
      ir_op = std::make_unique<IRSubOp>(op, std::vector<IRValue *>{ir_value.get(), left, right});
      break;

    case tok_mul:
      ir_op = std::make_unique<IRMulOp>(op, std::vector<IRValue *>{ir_value.get(), left, right});
      break;
    
    case tok_div:
      ir_op = std::make_unique<IRDivOp>(op, std::vector<IRValue *>{ir_value.get(), left, right});
      break;

    case tok_assign:
      ir_op = std::make_unique<IRAssignOp>(op, std::vector<IRValue *>{left, right});
  }

  addOp(std::move(ir_op));
  std::string id = std::to_string(ir_value->id());
  addValue(std::move(ir_value), id);
  return ir_value_;
}

IRValue *IRFunction::CreateIf(IRValue *cond, int *block_if, int *block_else) {
  *block_if = ++this_block;
  *block_else = ++this_block;
  std::unique_ptr<IRIfOp> ir_op = std::make_unique<IRIfOp>(*block_if, *block_else, 0, std::vector<IRValue *>{cond});
  addOp(std::move(ir_op));
  return nullptr;
}

IRValue *IRFunction::CreateGoto(int *block_end) {
  *block_end = ++this_block;
  std::unique_ptr<IRGotoOp> ir_op = std::make_unique<IRGotoOp>(*block_end, 0, std::vector<IRValue *>{});
  addOp(std::move(ir_op));
  return nullptr;
}

IRValue *IRFunction::CreateCall(std::string name, std::vector<IRValue *> args) {
  std::unique_ptr<IRValue> ir_value = std::make_unique<IRValue>(++this_id);
  IRValue *ir_value_ = ir_value.get();
  args.insert(args.begin(), ir_value_);
  std::unique_ptr<IROp> ir_op = std::make_unique<IRCallOp>(name, 0, args);
  addOp(std::move(ir_op));
  std::string id = std::to_string(ir_value->id());
  addValue(std::move(ir_value), id);
  return ir_value_;
}

IRValue *IRFunction::CreateRet(IRValue *value) {
  std::unique_ptr<IRRetOp> ir_op = std::make_unique<IRRetOp>(0, std::vector<IRValue *>{value});
  addOp(std::move(ir_op));
  return value;
}

IRValue *IRFunction::CreateList(std::string name, std::vector<double> values) {
  // name_to_list[name] = values;
  for (int i = 0; i < values.size(); ++i) {
    auto value = CreateValue(name + std::to_string(i) + "]", true);
    auto c = CreateNumber(values[i]);
    CreateBinary(value, c, tok_assign);
  }
  return nullptr;
}

IRValue *IRFunction::CreateSubValue(std::string name, int index) {
  // if (name_to_list.find(name) == name_to_list.end()) {
  //   abort();
  // }
  return CreateValue(name + std::to_string(index) + "]", false);
}

IROp::IROp(int op, std::vector<IRValue *> values)
    : op_(op), values_(std::move(values)) {
}

IROp::~IROp() {
}

std::string IROp::toString() {
  return "IROp\n";
}

std::string IRIfOp::toString() {
  std::stringstream ss;
  ss << "  goto L" << std::to_string(block_if_) << " if " << values_[0]->toString() << " else goto L" <<  std::to_string(block_else_) << "\n";
  return ss.str();
}

std::string IRGotoOp::toString() {
  std::stringstream ss;
  ss << "  goto L" << std::to_string(block_goto_) << "\n";
  return ss.str();
}

std::string IRCallOp::toString() {
  std::stringstream ss;
  ss << "  " << values_[0]->toString() << " = @" << name_ << " ";
  for (int i = 1; i < values_.size(); ++i) {
    ss << values_[i]->toString() << " ";
  }
  ss << "\n";
  return ss.str();
}

std::string IRLessOp::toString() {
  std::stringstream ss;
  ss << "  ";
  ss << values_[0]->toString();
  ss << " = fless ";
  ss << values_[1]->toString();
  ss << " ";
  ss << values_[2]->toString();
  ss << "\n";
  return ss.str();
}


std::string IRLessEqualOp::toString() {
  std::stringstream ss;
  ss << "  ";
  ss << values_[0]->toString();
  ss << " = flessequal ";
  ss << values_[1]->toString();
  ss << " ";
  ss << values_[2]->toString();
  ss << "\n";
  return ss.str();
}

std::string IRGreatOp::toString() {
  std::stringstream ss;
  ss << "  ";
  ss << values_[0]->toString();
  ss << " = fgreat ";
  ss << values_[1]->toString();
  ss << " ";
  ss << values_[2]->toString();
  ss << "\n";
  return ss.str();
}

std::string IRGreatEqualOp::toString() {
  std::stringstream ss;
  ss << "  ";
  ss << values_[0]->toString();
  ss << " = fgreatequal ";
  ss << values_[1]->toString();
  ss << " ";
  ss << values_[2]->toString();
  ss << "\n";
  return ss.str();
}

std::string IREqualOp::toString() {
  std::stringstream ss;
  ss << "  ";
  ss << values_[0]->toString();
  ss << " = fequal ";
  ss << values_[1]->toString();
  ss << " ";
  ss << values_[2]->toString();
  ss << "\n";
  return ss.str();
}

std::string IRAddOp::toString() {
  std::stringstream ss;
  ss << "  ";
  ss << values_[0]->toString();
  ss << " = fadd ";
  ss << values_[1]->toString();
  ss << " ";
  ss << values_[2]->toString();
  ss << "\n";
  return ss.str();
}

std::string IRSubOp::toString() {
  std::stringstream ss;
  ss << "  ";
  ss << values_[0]->toString();
  ss << " = fsub ";
  ss << values_[1]->toString();
  ss << " ";
  ss << values_[2]->toString();
  ss << "\n";
  return ss.str();
}

std::string IRMulOp::toString() {
  std::stringstream ss;
  ss << "  ";
  ss << values_[0]->toString();
  ss << " = fmul ";
  ss << values_[1]->toString();
  ss << " ";
  ss << values_[2]->toString();
  ss << "\n";
  return ss.str();
}

std::string IRDivOp::toString() {
  std::stringstream ss;
  ss << "  ";
  ss << values_[0]->toString();
  ss << " = fdiv ";
  ss << values_[1]->toString();
  ss << " ";
  ss << values_[2]->toString();
  ss << "\n";
  return ss.str();
}

std::string IRAssignOp::toString() {
  std::stringstream ss;
  ss << "  ";
  ss << values_[0]->toString();
  ss << " = ";
  ss << values_[1]->toString();
  ss << "\n";
  return ss.str();
}

std::string IRRetOp::toString() {
  std::stringstream ss;
  ss << "  ret " << values_[0]->toString() << "\n";
  return ss.str();
}

}  // namespace smcc
