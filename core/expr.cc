// Copyright (c) 2020 smarsufan. All Rights Reserved.

#include <utility>

#include "expr.h"

namespace smcc {

PrototypeExpr::PrototypeExpr(Token token, std::string name, std::vector<VarExpr> args)
    : token_(token), name_(name), args_(std::move(args)) {
}

void *PrototypeExpr::codegen() {
}

FunctionExpr::FunctionExpr(std::unique_ptr<PrototypeExpr> proto, std::vector<std::unique_ptr<Expr>> body)
    : proto_(std::move(proto)), body_(std::move(body)) {
}

void *FunctionExpr::codegen() {
}

IfExpr::IfExpr(std::unique_ptr<Expr> cond, std::vector<std::unique_ptr<Expr>> body, std::vector<std::unique_ptr<Expr>> other)
    : cond_(std::move(cond)), body_(std::move(body)), other_(std::move(other)) {
}

void* IfExpr::codegen() {
}

VarExpr::VarExpr(Token token, std::string name)
    : token_(token), name_(name) {  // It's danger to move.
}

void *VarExpr::codegen() {

}

NumberExpr::NumberExpr(double num_val)
    : num_val_(num_val) {
}

void *NumberExpr::codegen() {
}

BinaryExpr::BinaryExpr(Token tok, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
    : tok_(tok), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {
}

void *BinaryExpr::codegen() {
}

CallExpr::CallExpr(std::string id, std::vector<std::unique_ptr<Expr>> args)
    : id_(id), args_(args) {
}

void *CallExpr::codegen() {
}

}  // namespace smcc
