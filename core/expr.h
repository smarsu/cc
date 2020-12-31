// Copyright (c) 2020 smarsufan. All Rights Reserved.

#pragma once
#include <vector>
#include <string>
#include <memory>

#include "ast.h"

namespace smcc {

class Expr {
 public:
  Expr() = default;

  virtual ~Expr() = default;

  virtual void *codegen() = 0;
};

class PrototypeExpr : public Expr {
 public:
  PrototypeExpr(Token token, std::string name, std::vector<Expr> args);

  virtual void *codegen();

 private:
  Token token_;
  std::string name_;
  std::vector<VarExpr> args_;
};

class FunctionExpr : public Expr {
 public:
  FunctionExpr(std::unique_ptr<PrototypeExpr> proto, std::vector<std::unique_ptr<Expr>> body);

  virtual void *codegen();

 private:
  std::unique_ptr<PrototypeExpr> proto_;
  std::vector<std::unique_ptr<Expr>> body_;
};

class IfExpr : public Expr {
 public:
  IfExpr(std::unique_ptr<Expr> cond, std::vector<std::unique_ptr<Expr>> body, std::vector<std::unique_ptr<Expr>> other);

  virtual void *codegen();

 private:
  std::unique_ptr<Expr> cond_;
  std::vector<std::unique_ptr<Expr>> body_;
  std::vector<std::unique_ptr<Expr>> other_;
};

class VarExpr : public Expr {
 public:
  VarExpr(Token token, std::string name);

  virtual void *codegen();

 private:
  Token token_;
  std::string name_;
};

class NumberExpr : public Expr {
 public:
  NumberExpr(double num_val);

  virtual void *codegen();

 private:
  double num_val_;
};

class BinaryExpr : public Expr {
 public:
  BinaryExpr(Token tok, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs);

  virtual void *codegen();

 private:
  Token tok_;
  std::unique_ptr<Expr> lhs_;
  std::unique_ptr<Expr> rhs_;
};

class CallExpr : public Expr {
 public:
  CallExpr(std::string id, std::vector<std::unique_ptr<Expr>> args);

  virtual void *codegen();

 private:
  std::string id_;
  std::vector<std::unique_ptr<Expr>> args_;
};

}  // namespace smcc
