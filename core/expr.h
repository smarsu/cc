// Copyright (c) 2020 smarsufan. All Rights Reserved.

#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>

#include "ir.h"

// #include "ast.h"

namespace smcc {

void reset();
double call(const std::string &func_id, const std::vector<double> &args);

IRBuilder *ir_builder();

class Expr {
 public:
  Expr() = default;

  virtual ~Expr() = default;

  virtual int run() = 0;

  virtual IRValue *codegen() = 0;

  virtual int this_stack_idx() { return this_stack_idx_; }

 protected:
  int this_stack_idx_ = -1;
};

class VarExpr : public Expr {
 public:
  VarExpr(int token, std::string name);

  virtual int run();

  virtual IRValue *codegen();

  virtual int this_stack_idx();

 public:
  int token_;
  std::string name_;
  std::map<int, int> prefix_to_index_;
};

class SubVarExpr : public Expr {
 public:
  SubVarExpr(std::string name, int index);

  virtual int run();

  virtual IRValue *codegen();

  // virtual int this_stack_idx();

 public:
  std::string name_;
  int index_;
  // std::map<int, int> prefix_to_index_;
};

class PrototypeExpr : public Expr {
 public:
  PrototypeExpr(int token, std::string name, std::vector<std::unique_ptr<VarExpr>> args);

  virtual int run();

  virtual IRValue *codegen();

 public:
  int token_;
  std::string name_;
  std::vector<std::unique_ptr<VarExpr>> args_;
};

class BodyExpr : public Expr {
 public:
  BodyExpr(std::vector<std::unique_ptr<Expr>> body);

  virtual int run();

  virtual IRValue *codegen();

 public:
  std::vector<std::unique_ptr<Expr>> body_;
};

class FunctionExpr : public Expr {
 public:
  FunctionExpr(std::unique_ptr<PrototypeExpr> proto, std::unique_ptr<BodyExpr> body);

  virtual int run();

  virtual IRValue *codegen();

 public:
  std::unique_ptr<PrototypeExpr> proto_;
  std::unique_ptr<BodyExpr> body_;
};

class IfExpr : public Expr {
 public:
  IfExpr(std::unique_ptr<Expr> cond, std::unique_ptr<Expr> body, std::unique_ptr<Expr> other);

  virtual int run();

  virtual IRValue *codegen();

 public:
  std::unique_ptr<Expr> cond_;
  std::unique_ptr<Expr> body_;
  std::unique_ptr<Expr> other_;
};

class WhileExpr : public Expr {
 public:
  WhileExpr(std::unique_ptr<Expr> cond, std::unique_ptr<Expr> body);

  virtual int run();

  virtual IRValue *codegen();

 public:
  std::unique_ptr<Expr> cond_;
  std::unique_ptr<Expr> body_;
};

class NumberExpr : public Expr {
 public:
  NumberExpr(double num_val);

  virtual int run();

  virtual IRValue *codegen();

 public:
  double num_val_;
};

class ListExpr : public Expr {
 public:
  ListExpr(std::string name, std::vector<double> num_vals);

  virtual int run();

  virtual IRValue *codegen();

 public:
  std::string name_;
  std::vector<double> num_vals_;
};

class BinaryExpr : public Expr {
 public:
  BinaryExpr(int tok, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs);

  virtual int run();

  virtual IRValue *codegen();

 public:
  int tok_;
  std::unique_ptr<Expr> lhs_;
  std::unique_ptr<Expr> rhs_;
};

class CallExpr : public Expr {
 public:
  CallExpr(std::string id, std::vector<std::unique_ptr<Expr>> args);

  virtual int run();

  virtual IRValue *codegen();

 public:
  std::string id_;
  std::vector<std::unique_ptr<Expr>> args_;
};

class ReturnExpe : public Expr {
 public:
  ReturnExpe(std::unique_ptr<Expr> expr);

  virtual int run();

  virtual IRValue *codegen();

 public:
  std::unique_ptr<Expr> expr_;
};

}  // namespace smcc
