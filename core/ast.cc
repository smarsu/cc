// Copyright (c) 2020 smarsufan. All Rights Reserved.

#include "ast.h"
#include <iostream>
#include <memory>
#include <utility>

// namespace std {
// template <typename T>
// using Ele = typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T> >::type;

// template <typename T, typename ... Args> inline
// Ele<T> make_unique(Args && ... args) {
//     return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
// }

// }  // namespace std

namespace smcc {

AST::AST(Reader *reader)
    : reader_(reader) {

}

AST::~AST() {
  
}

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
    if (identifier_str == "if")
      return tok_if;
    if (identifier_str == "else")
      return tok_else;
    if (identifier_str == "int")
      return tok_int;
    if (identifier_str == "double")
      return tok_dbl;
    if (identifier_str == "return")
      return tok_return;
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
      return tok_div;
    }

    do
      last_char = getchar();
    while (last_char != EOF && last_char != '\n' && last_char != '\r');

    if (last_char != EOF)
      return gettok();
  }

  if (last_char == '<') {
    last_char = getchar();
    if (last_char == '=') {
      last_char = getchar();

      return tok_lessequal;
    }
    else {
      return tok_less;
    }
  }

  if (last_char == '>') {
    last_char = getchar();
    if (last_char == '=') {
      last_char = getchar();

      return tok_greatequal;
    }
    else {
      return tok_great;
    }
  }

  if (last_char == '=') {
    last_char = getchar();
    if (last_char == '=') {
      last_char = getchar();

      return tok_equal;
    }
    else {
      return tok_assign;
    }
  }

  if (last_char == '+') {
    last_char = getchar();
    return tok_add;
  }

  if (last_char == '-') {
    last_char = getchar();
    return tok_sub;
  }

  if (last_char == '*') {
    last_char = getchar();
    return tok_mul;
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
    // if (!isascii(cur_tok))
    //   return -1;
    if (binop_precedence.find(cur_tok) == binop_precedence.end()) {
      return -1;
    }

    // Make sure it's a declared binop.
    int tok_prec = binop_precedence[cur_tok];
    if (tok_prec <= 0)
      return -1;
    return tok_prec;
}

void AST::parse() {
  getNextToken();

  // std::vector<std::unique_ptr<Expr>> exprs;

  while (cur_tok != tok_eof) {
    if (cur_tok <= tok_dt) {
      fprintf(stderr, "need tok type -100");
      abort();
    }

    // Token type = curtok();
    cur_type = curtok();
    getNextToken();  // eat type

    if (cur_tok != tok_identifier) {
      fprintf(stderr, "-200");
      abort();
    }
    auto name = identifier_str;
    getNextToken();  // eat id
    if (cur_tok == '(') {
      // getNextToken();  // eat (
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
    fprintf(stderr, "-300");
    abort();
  }

  getNextToken();  // eat '('

  std::vector<std::unique_ptr<VarExpr>> args;
  while (cur_tok != ')') {
    if (cur_tok <= tok_dt) {
      fprintf(stderr, "-400");
      abort();
    }

    cur_type = curtok();
    getNextToken();  // eat type

    if (cur_tok != tok_identifier) {
      fprintf(stderr, "-500");
      abort();
    }
    auto name = identifier_str;

    auto arg = std::make_unique<VarExpr>(cur_type, name);
    args.emplace_back(std::move(arg));

    getNextToken();  // eat name
    if (cur_tok != ',' && cur_tok != ')') {
      fprintf(stderr, "-600");
      abort();
    }

    if (cur_tok == ',') {
      getNextToken();  // eat ,
    }
  }

  auto def = std::make_unique<PrototypeExpr>(def_token, def_name, std::move(args));

  getNextToken();  // eat )

  if (cur_tok != '{') {
    fprintf(stderr, "-700");
    abort();
  }

  auto body = ParseBody();

  auto func = std::make_unique<FunctionExpr>(std::move(def), std::move(body));
  return std::move(func);
}

std::vector<std::unique_ptr<Expr>> AST::ParseVars(Token token, const std::string &name) {
  std::unique_ptr<VarExpr> var = std::make_unique<VarExpr>(token, name);

  std::vector<std::unique_ptr<Expr>> exprs;

  auto expr = ParseBinaryOp(0, std::move(var));
  exprs.push_back(std::move(expr));

  while (cur_tok != ';') {
    if (cur_tok != ',') {
      fprintf(stderr, "-800");
      abort();
    }

    getNextToken();  // eat ','
    if (cur_tok <= tok_dt) {
      fprintf(stderr, "-810");      
      abort();
    }

    cur_type = curtok();

    getNextToken();  // eat dt
    if (cur_tok != tok_identifier) {
      fprintf(stderr, "-820");      
      abort();
    }

    auto cur_name = identifier_str;

    std::unique_ptr<VarExpr> var = std::make_unique<VarExpr>(cur_type, cur_name);
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
      // Token type = curtok();
      getNextToken();  // eat dt
      if (cur_tok != tok_identifier) {
        fprintf(stderr, "-900");
        abort();
      }
      auto name = identifier_str;
      auto vars = ParseVars(cur_type, name);
      for (auto &var : vars) {
        exprs.push_back(std::move(var));
      }
    }
    else if (cur_tok == tok_if) {
      auto expr = ParseIf();
      exprs.push_back(std::move(expr));
    }
    else if (cur_tok == tok_return) {
      getNextToken();  // eat return
      auto expr = ParseExpression();
      exprs.push_back(std::make_unique<ReturnExpe>(std::move(expr)));

      if (cur_tok != ';') {
        fprintf(stderr, "-910");
        abort();
      }

      getNextToken();  // eat ;
    }
    else {
      auto expr = ParseExpression();
      exprs.push_back(std::move(expr));

      if (cur_tok != ';') {
        fprintf(stderr, "-920");
        abort();
      }

      getNextToken();  // eat ;
    }
  }

  getNextToken();  // eat }
  // return std::move(exprs);
  return exprs;

}

std::unique_ptr<Expr> AST::ParseIf() {
  getNextToken();  // eat if
  if (cur_tok != '(') {
    fprintf(stderr, "-1000");
    abort();
  }
  // getNextToken();  // eat (
  auto cond = ParseExpression();
  if (cur_tok != '{') {
    fprintf(stderr, "-1200");
    abort();
  }
  auto body = ParseBody();
  std::vector<std::unique_ptr<Expr>> other;
  if (cur_tok == tok_else) {
    getNextToken();  // eat else
    if (cur_tok == tok_if) {
      auto expr = ParseIf();
      other.push_back(std::move(expr));
    }
    else {
      // other = std::move(ParseBody());
      other = ParseBody();
    }
  }

  return std::make_unique<IfExpr>(std::move(cond), std::move(body), std::move(other));
}

std::unique_ptr<Expr> AST::ParseExpression() {
  auto lhs = ParsePrimary();
  return ParseBinaryOp(0, std::move(lhs));
}

std::unique_ptr<Expr> AST::ParsePrimary() {
  if (cur_tok == tok_identifier) {
    // ParseIdentifierExpr
    auto name = identifier_str;
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
          fprintf(stderr, "-1400");
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
      fprintf(stderr, "-1500");
      abort();
    }
    getNextToken();  // eat ')'
    return expr;
  }
  else {
    fprintf(stderr, "-1600");
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
      fprintf(stderr, "-1700");
      abort();
    }

    int next_prec = GetTokPrecedence();
    if (tok_prec < next_prec) {
      rhs = ParseBinaryOp(tok_prec + 1, std::move(rhs));
      if (!rhs) {
        fprintf(stderr, "-1800");
        abort();
      }
    }

    lhs = std::make_unique<BinaryExpr>(binop, std::move(lhs), std::move(rhs));
  }
}

}  // namespace smcc
