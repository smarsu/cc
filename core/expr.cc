// Copyright (c) 2020 smarsufan. All Rights Reserved.

#include <utility>
#include <map>
#include <cmath>

#include "expr.h"
#include "ast.h"

namespace smcc {

static IRBuilder Builder;

std::vector<double> stack(8 * 1024 * 1024);

int prefix = 0;
std::map<std::string, int> vars;
std::map<std::string, FunctionExpr *> funcs;
int stack_idx = 0;
int this_func_idx = 0;
int find_return = 0;

void reset() {
  prefix = 0;
  vars.clear();
//   funcs.clear();
  stack_idx = 0;
  this_func_idx = 0;
  find_return = 0;
}

double call(const std::string &func_id, const std::vector<double> &args) {
  if (funcs.find(func_id) != funcs.end()) {
    funcs[func_id]->proto_->run();
    for (int idx = 0; idx < args.size(); ++idx) {
      // int proto_index = funcs[func_id]->proto_->args_[idx]->this_stack_idx_;
      int proto_index = funcs[func_id]->proto_->args_[idx]->this_stack_idx();
      stack[proto_index] = args[idx];
    }
    int index = funcs[func_id]->run();
    return stack[index];
  }
  else {
    fprintf(stderr, "expr -1000\n");
    abort();
  }
}

IRBuilder *ir_builder() {
  return &Builder;
}

PrototypeExpr::PrototypeExpr(int token, std::string name, std::vector<std::unique_ptr<VarExpr>> args)
    : token_(token), name_(name), args_(std::move(args)) {
}

int PrototypeExpr::run() {
  for (auto &arg : args_) {
    arg->run();
  }
  return -1;
}

IRValue *PrototypeExpr::codegen() {
  return nullptr;
}

BodyExpr::BodyExpr(std::vector<std::unique_ptr<Expr>> body)
    : body_(std::move(body)) {
}

int BodyExpr::run() {
  int isFind = find_return;
  int index = -1;
  for (auto &b : body_) {
    index = b->run();
    if (isFind != find_return) {
      break;
    }
  }
  return index;
}

IRValue *BodyExpr::codegen() {
  int id = Builder.id();
  for (auto &body : body_) {
    body->codegen();
  }
  Builder.setId(id);  // It will erase the var in the loop
  return nullptr;
}

FunctionExpr::FunctionExpr(std::unique_ptr<PrototypeExpr> proto, std::unique_ptr<BodyExpr> body)
    : proto_(std::move(proto)), body_(std::move(body)) {
  funcs[proto_->name_] = this;
}

int FunctionExpr::run() {
  // for (auto &b : body_) {
  //   this_stack_idx_ = b->run();
  //   if (find_return) {
  //     break;
  //   }
  // }
  // this_stack_idx_ = runExprs(body_);
  body_->run();
  // find_return = false;
  --find_return;
  return this_stack_idx_;
}

IRValue *FunctionExpr::codegen() {
  std::vector<std::string> args_name_(proto_->args_.size());
  for (int i = 0; i < proto_->args_.size(); ++i) {
    args_name_[i] = proto_->args_[i]->name_;
  }
  Builder.addFunction(proto_->name_, args_name_);
  // for (auto &body : body_) {
  //   body->codegen();
  // }
  body_->codegen();
  return nullptr;
}

IfExpr::IfExpr(std::unique_ptr<Expr> cond, std::unique_ptr<Expr> body, std::unique_ptr<Expr> other)
    : cond_(std::move(cond)), body_(std::move(body)), other_(std::move(other)) {
}

int IfExpr::run() {
  if (stack[cond_->run()]) {
    // for (auto &b : body_) {
    //   this_stack_idx_ = b->run();
    //   if (find_return) {
    //     break;
    //   }
    // }
    // this_stack_idx_ = runExprs(body_);
    body_->run();
  }
  else {
    // for (auto &o : other_) {
    //   this_stack_idx_ = o->run();
    //   if (find_return) {
    //     break;
    //   }
    // }
    // this_stack_idx_ = runExprs(other_);
    other_->run();
  }
  return this_stack_idx_;
}

IRValue *IfExpr::codegen() {
  auto *value = cond_->codegen();
  int block_if;
  int block_else;
  Builder.fuction()->CreateIf(value, &block_if, &block_else);
  int pos_if = Builder.fuction()->op_size();
  body_->codegen();
  int block_end;
  Builder.fuction()->CreateGoto(&block_end);
  int pos_else = Builder.fuction()->op_size();
  if (other_) other_->codegen();
  int pos_end = Builder.fuction()->op_size();
  Builder.fuction()->setGoto(block_if, pos_if);
  Builder.fuction()->setGoto(block_else, pos_else);
  Builder.fuction()->setGoto(block_end, pos_end);
  return nullptr;
}

WhileExpr::WhileExpr(std::unique_ptr<Expr> cond, std::unique_ptr<Expr> body) 
    : cond_(std::move(cond)), body_(std::move(body)) {
}

int WhileExpr::run() {
  return -1;
}

IRValue *WhileExpr::codegen() {
  int pos_while = Builder.fuction()->op_size();
  auto *value = cond_->codegen();
  int block_if;
  int block_else;
  Builder.fuction()->CreateIf(value, &block_if, &block_else);
  int pos_if = Builder.fuction()->op_size();
  body_->codegen();
  int block_while;
  Builder.fuction()->CreateGoto(&block_while);
  int pos_else = Builder.fuction()->op_size();
  Builder.fuction()->setGoto(block_if, pos_if);
  Builder.fuction()->setGoto(block_else, pos_else);
  Builder.fuction()->setGoto(block_while, pos_while);
  return nullptr;
}

VarExpr::VarExpr(int token, std::string name)
    : token_(token), name_(name) {  // It's danger to move.
}

int VarExpr::this_stack_idx() {
  return prefix_to_index_[prefix];
}

int VarExpr::run() {
  std::string name = std::to_string(prefix) + name_;
  if (vars.find(name) != vars.end()) {
    // Found It
    if (token_ != tok_dt) {
      fprintf(stderr, "expr -2000\n");
      abort();
    }
    prefix_to_index_[prefix] = vars[name];
    // this_stack_idx_ = vars[name];
  }
  else {
    // Not Found
    if (token_ <= tok_dt) {
      fprintf(stderr, "expr -3000\n");
      abort();
    }
    // this_stack_idx_ = ++stack_idx;
    // vars[name] = this_stack_idx_;
    prefix_to_index_[prefix] = ++stack_idx;
    vars[name] = prefix_to_index_[prefix];
  }

  // return this_stack_idx_;
  return prefix_to_index_[prefix];
}

IRValue *VarExpr::codegen() {
  auto value = Builder.fuction()->CreateValue(name_, token_ != tok_dt);
  return value;
}

SubVarExpr::SubVarExpr(std::string name, int index) 
    : name_(name), index_(index) {
}

int SubVarExpr::run() {
  return -1;
}

IRValue *SubVarExpr::codegen() {
  return Builder.fuction()->CreateSubValue(name_, index_);
}

NumberExpr::NumberExpr(double num_val)
    : num_val_(num_val) {
}

int NumberExpr::run() {
  this_stack_idx_ = ++stack_idx;
  stack[this_stack_idx_] = num_val_;
  return this_stack_idx_;
}

IRValue *NumberExpr::codegen() {
  return Builder.fuction()->CreateNumber(num_val_);
}

ListExpr::ListExpr(std::string name, std::vector<double> num_vals)
    : name_(name), num_vals_(std::move(num_vals)) {
}

int ListExpr::run() {
  return -1;
}

IRValue *ListExpr::codegen() {
  return Builder.fuction()->CreateList(name_, num_vals_);
}

BinaryExpr::BinaryExpr(int tok, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
    : tok_(tok), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {
}

int BinaryExpr::run() {
  auto lhs = lhs_->run();
  auto rhs = rhs_->run();

  this_stack_idx_ = stack_idx + 1;

  switch (tok_) {
    default:
      fprintf(stderr, "expr -4000\n");
      abort();

    case tok_less:
      stack[++stack_idx] = stack[lhs] < stack[rhs];
      break;

    case tok_lessequal:
      stack[++stack_idx] = stack[lhs] <= stack[rhs];
      break;

    case tok_great:
      stack[++stack_idx] = stack[lhs] > stack[rhs];
      break;

    case tok_greatequal:
      stack[++stack_idx] = stack[lhs] >= stack[rhs];
      break;

    case tok_equal:
      stack[++stack_idx] = stack[lhs] == stack[rhs];
      break;

    case tok_add:
      stack[++stack_idx] = stack[lhs] + stack[rhs];
      break;

    case tok_sub:
      stack[++stack_idx] = stack[lhs] - stack[rhs];
      break;

    case tok_mul:
      stack[++stack_idx] = stack[lhs] * stack[rhs];
      break;

    case tok_div:
      stack[++stack_idx] = stack[lhs] / stack[rhs];
      break;

    case tok_assign:
      stack[++stack_idx] = (stack[lhs] = stack[rhs]);
      break;
  }

  return this_stack_idx_;
}

IRValue *BinaryExpr::codegen() {
  auto value = Builder.fuction()->CreateBinary(lhs_->codegen(), rhs_->codegen(), tok_);
  return value;
}

CallExpr::CallExpr(std::string id, std::vector<std::unique_ptr<Expr>> args)
    : id_(id), args_(std::move(args)) {
}

int CallExpr::run() {
  this_stack_idx_ = ++stack_idx;

  if (funcs.find(id_) != funcs.end()) {
    std::vector<double> args_values(args_.size());
    for (int idx = 0; idx < args_.size(); ++idx) {
      args_values[idx] = stack[args_[idx]->run()];
    }
    ++prefix;
    for (int idx = 0; idx < args_.size(); ++idx) {
      int proto_index = funcs[id_]->proto_->args_[idx]->run();
      // stack[proto_index] = stack[args_[idx]->run()];
      stack[proto_index] = args_values[idx];
    }
    int idx = funcs[id_]->run();
    stack[this_stack_idx_] = stack[idx];
    --prefix;
  }
  else if (id_ == "sqrt") {
    // The args have not been run this time, so we should run it.
    double value = std::sqrt(stack[args_[0]->run()]);
    stack[this_stack_idx_] = value;
  }
  else if (id_ == "sin") {
    double value = std::sin(stack[args_[0]->run()]);
    stack[this_stack_idx_] = value;
  }
  else if (id_ == "pow") {
    double value = std::pow(stack[args_[0]->run()], stack[args_[1]->run()]);
    stack[this_stack_idx_] = value;
  }
  else {
    fprintf(stderr, "expr -5000: %s\n", id_.c_str());
    abort();
  }

  return this_stack_idx_;
}

IRValue *CallExpr::codegen() {
  std::vector<IRValue *> args(args_.size());
  for (int i = 0; i < args_.size(); ++i) {
    args[i] = args_[i]->codegen();
  }
  return Builder.fuction()->CreateCall(id_, args);
}

ReturnExpe::ReturnExpe(std::unique_ptr<Expr> expr)
    : expr_(std::move(expr)) {
}

int ReturnExpe::run() { 
  // find_return = true;
  ++find_return;
  return expr_->run();
}

IRValue *ReturnExpe::codegen() {
  return Builder.fuction()->CreateRet(expr_->codegen());
  // return nullptr;
}

}  // namespace smcc
