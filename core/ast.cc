// Copyright (c) 2020 smarsufan. All Rights Reserved.

#include "ast.h"
#include <memory>
#include <utility>

namespace smcc {

int AST::gettok() {
  // Skip any whitespace.
  while (isspace(last_char))
    last_char = getchar();

  if (isalpha(last_char)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    identifier_str = last_char;
    while (isalnum((last_char = getchar())))
      identifier_str += last_char;

    if (identifier_str == "def")
      return tok_def;
    if (identifier_str == "extern")
      return tok_extern;
    return tok_identifier;
  }

  if (isdigit(last_char) || last_char == '.') { // Number: [0-9.]+
    std::string num_str;
    do {
      num_str += last_char;
      last_char = getchar();
    } while (isdigit(last_char) || last_char == '.');

    num_val = strtod(num_str.c_str(), nullptr);
    return tok_number;
  }

  if (last_char == '/') {
    // Comment until end of line.
    last_char = getchar();
    if (last_char != '/') {
      return '/';
    }

    do
      last_char = getchar();
    while (last_char != EOF && last_char != '\n' && last_char != '\r');

    if (last_char != EOF)
      return gettok();
  }

  // Check for end of file.  Don't eat the EOF.
  if (last_char == EOF)
    return tok_eof;

  // Otherwise, just return the character as its ascii value.
  int this_char = last_char;
  last_char = getchar();
  return this_char;
}

int AST::GetTokPrecedence() {
    if (!isascii(cur_tok))
      return -1;

    // Make sure it's a declared binop.
    int tok_prec = binop_precedence[cur_tok];
    if (tok_prec <= 0)
      return -1;
    return tok_prec;
}

void AST::parse() {
  getNextToken();

  std::vector<std::unique_ptr<Expr>> exprs;

  while (cur_tok != tok_eof) {
    if (cur_tok <= tok_dt) {
      abort();
    }

    Token type = curtok();
    cur_type = curtok();
    getNextToken();  // eat type

    if (cur_tok != tok_identifier) {
      abort();
    }
    auto name = identifier_str;
    if (cur_tok == '(') {
      getNextToken();  // eat ()
      auto expr = ParseDefinition(cur_type, name);
      exprs.push_back(std::move(expr));
    }
    else {
      auto vars = ParseVars(cur_type, name);
      for (auto &var : vars) {
        exprs.push_back(std::move(var));
      }
    }
  }
}

std::unique_ptr<Expr> AST::ParseDefinition(Token def_token, const std::string &def_name) {
  if (cur_tok != '(') {
    abort();
  }

  getNextToken();  // eat '('

  std::vector<std::unique_ptr<Expr>> args;
  while (cur_tok != ')') {
    if (cur_tok <= tok_dt) {
      abort();
    }

    cur_type = curtok();
    getNextToken();  // eat type

    if (cur_tok != tok_identifier) {
      abort();
    }
    auto name = identifier_str;

    auto arg = std::make_unique<VarExpr>(cur_type, name);
    args.push_back(std::move(arg));

    getNextToken();  // eat name
    if (cur_tok != ',' && cur_tok != ')') {
      abort();
    }
  }

  auto def = std::make_unique<PrototypeExpr>(def_token, def_name, args);

  getNextToken();  // eat )

  if (cur_tok != '{') {
    abort();
  }

  auto body = ParseBody();

  auto func = std::make_unique<FunctionExpr>(def, body);
  return func;
}

std::vector<std::unique_ptr<Expr>> AST::ParseVars(Token token, const std::string &name) {
  std::unique_ptr<VarExpr> var = std::make_unique<VarExpr>(token, name);

  std::vector<std::unique_ptr<Expr>> exprs;

  auto expr = ParseBinaryOp(0, std::move(var));
  exprs.push_back(std::move(expr));

  while (cur_tok != ';') {
    if (cur_tok != ',') {
      abort();
    }

    std::unique_ptr<VarExpr> var = std::make_unique<VarExpr>(token, name);
    auto expr = ParseBinaryOp(0, std::move(var));
    exprs.push_back(std::move(expr));
  }

  return exprs;
}

std::vector<std::unique_ptr<Expr>> AST::ParseBody() {
  std::vector<std::unique_ptr<Expr>> exprs;
  getNextToken();  // eat {
  while (cur_tok != '}') {
    if (cur_tok == tok_dt) {
      Token type = curtok();
      getNextToken();  // eat dt
      if (cur_tok != tok_identifier) {
        abort();
      }
      auto name = identifier_str;
      auto vars = ParseVars(cur_type, name);
      exprs.insert(exprs.begin(), vars.begin(), vars.end());
    }
    else if (cur_tok == tok_if) {
      auto expr = ParseIf();
      exprs.push_back(std::move(expr));
    }
    else {
      auto expr = ParseExpression();
      exprs.push_back(std::move(expr));
    }
  }
  return exprs;
}

std::unique_ptr<Expr> AST::ParseIf() {
  getNextToken();  // eat if
  if (cur_tok != '(') {
    abort();
  }
  // getNextToken();  // eat (
  auto cond = ParseExpression();
  if (cur_tok != ')') {
    abort();
  }
  getNextToken();  // eat )
  if (cur_tok != '{') {
    abort();
  }
  auto body = ParseBody();
  if (cur_tok != '}') {
    abort();
  }
  getNextToken();  // eat }
  std::vector<std::unique_ptr<Expr>> other;
  if (cur_tok == tok_else) {
    getNextToken();  // eat else
    if (cur_tok == tok_if) {
      other = std::vector<std::unique_ptr<Expr>>{ParseIf()};
    }
    else {
      other = ParseBody();
    }
  }

  return std::make_unique<IfExpr>(cond, body, other);
}

std::unique_ptr<Expr> AST::ParseExpression() {
  auto lhs = ParsePrimary();
  return ParseBinaryOp(0, std::move(lhs));
}

std::unique_ptr<Expr> AST::ParsePrimary() {
  if (cur_tok == tok_identifier) {
    // ParseIdentifierExpr
    auto &name = identifier_str;
    getNextToken(); // eat id
    if (cur_tok != '(') {
      return std::make_unique<VarExpr>(tok_dt, name);
    }

    getNextToken(); // eat (
    std::vector<std::unique_ptr<Expr>> args;
    if (cur_tok != ')') {
      while (true) {
        auto arg = ParseExpression();
        args.push_back(std::move(arg));

        if (cur_tok == ')')
          break;

        if (cur_tok != ',') {
          abort();
        }
        getNextToken();  // eat ','
      }
    }

    // Eat the ')'.
    getNextToken();

    return std::make_unique<CallExpr>(name, std::move(args));
  }
  else if (cur_tok == tok_number) {
    auto expr = std::make_unique<NumberExpr>(num_val);
    getNextToken(); // eat the number
    return std::move(expr);
  }
  else if (cur_tok == '(') {
    getNextToken();  // eat '('
    auto expr = ParseExpression();
    if (cur_tok != ')') {
      abort();
    }
    getNextToken();  // eat ')'
    return expr;
  }
  else {
    abort();
  }
}

std::unique_ptr<Expr> AST::ParseBinaryOp(int expr_prec, std::unique_ptr<Expr> lhs) {
  while (true) {
    int tok_prec = GetTokPrecedence();

    if (tok_prec < expr_prec) {
      return lhs;
    }

    int binop = cur_tok;
    getNextToken(); // eat binop

    auto rhs = ParsePrimary();
    if (!rhs) {
      abort();
    }

    int next_prec = GetTokPrecedence();
    if (tok_prec < next_prec) {
      rhs = ParseBinaryOp(tok_prec + 1, std::move(rhs));
      if (!rhs) {
        abort();
      }
    }

    lhs = std::make_unique<BinaryExpr>(binop, std::move(lhs), std::move(rhs));
  }
}

}  // namespace smcc
