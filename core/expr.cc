// Copyright (c) 2020 smarsufan. All Rights Reserved.

#include <utility>
#include <map>
#include <cmath>

#include "expr.h"
#include "ast.h"

namespace smcc {

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

int runExprs(const std::vector<std::unique_ptr<Expr>> &body) {
  int isFind = find_return;
  int index = -1;
  for (auto &b : body) {
    index = b->run();
    if (isFind != find_return) {
      break;
    }
  }
  return index;
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

FunctionExpr::FunctionExpr(std::unique_ptr<PrototypeExpr> proto, std::vector<std::unique_ptr<Expr>> body)
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
  this_stack_idx_ = runExprs(body_);
  // find_return = false;
  --find_return;
  return this_stack_idx_;
}

IfExpr::IfExpr(std::unique_ptr<Expr> cond, std::vector<std::unique_ptr<Expr>> body, std::vector<std::unique_ptr<Expr>> other)
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
    this_stack_idx_ = runExprs(body_);
  }
  else {
    // for (auto &o : other_) {
    //   this_stack_idx_ = o->run();
    //   if (find_return) {
    //     break;
    //   }
    // }
    this_stack_idx_ = runExprs(other_);
  }
  return this_stack_idx_;
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

NumberExpr::NumberExpr(double num_val)
    : num_val_(num_val) {
}

int NumberExpr::run() {
  this_stack_idx_ = ++stack_idx;
  stack[this_stack_idx_] = num_val_;
  return this_stack_idx_;
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

ReturnExpe::ReturnExpe(std::unique_ptr<Expr> expr)
    : expr_(std::move(expr)) {
}

int ReturnExpe::run() { 
  // find_return = true;
  ++find_return;
  return expr_->run();
}

}  // namespace smcc
