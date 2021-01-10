// Copyright (c) 2020 smarsufan. All Rights Reserved.

#pragma once
#include <string>
#include <map>

#include "reader.h"
#include "expr.h"

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
  tok_nb = -256,
  
  tok_eof,

  // commands
  tok_def,
  tok_extern,

  // primary
  tok_identifier,
  tok_number,

  // sm
  tok_if,
  tok_else,
  tok_return,

  // binary
  tok_less,
  tok_lessequal,
  tok_great,
  tok_greatequal,
  tok_equal,

  tok_add,
  tok_sub,
  tok_mul,
  tok_div,

  tok_assign,

  // dtype
  tok_dt,
  tok_int,
  tok_dbl,
};

static std::map<char, int> binop_precedence = {
  // {'<', 10},
  // {'+', 20},
  // {'-', 20},
  // {'*', 40},
  {tok_assign, 10},
  {tok_less, 20},
  {tok_lessequal, 20},
  {tok_great, 20},
  {tok_greatequal, 20},
  {tok_equal, 20},
  {tok_add, 30},
  {tok_sub, 30},
  {tok_mul, 40},
  {tok_div, 40},
};

typedef struct TokenInfo {
    char name[16];
} TokenInfo;

/** this table gives more information about tokens */
static const TokenInfo token_info_[-tok_nb] = {
    [-tok_nb+tok_eof]        = { .name =  "eof"        },
    [-tok_nb+tok_def]        = { .name =  "def"        },
    [-tok_nb+tok_extern]     = { .name =  "extern"     },
    [-tok_nb+tok_identifier] = { .name =  "identifier" },
    [-tok_nb+tok_number]     = { .name =  "number"     }
};

static const TokenInfo *token_info = token_info_ - tok_nb;

namespace smcc {

/// The abstract syntax tree.
class AST {
 public:
  AST(Reader *reader);

  ~AST();

  int getNextToken() { return cur_tok = gettok(); }

  // get a tok.
  int gettok();

  void codegen();

  void parse();

  Token curtok() { return static_cast<Token>(cur_tok); }

 private:
  Reader *reader() {
    return reader_;
  }

  int getchar() {
    return reader()->getchar();
  }

  int GetTokPrecedence();

  std::unique_ptr<Expr> ParseDefinition(Token token, const std::string &name);

  std::unique_ptr<BodyExpr> ParseBody();

  std::unique_ptr<Expr> ParseIf();

  std::unique_ptr<Expr> ParsePrimary();

  std::vector<std::unique_ptr<Expr>> ParseVars(Token token, const std::string &name);

  std::unique_ptr<Expr> ParseExpression();

  std::unique_ptr<Expr> ParseBinaryOp(int expr_prec, std::unique_ptr<Expr> lhs);

 private:
  Reader *reader_{nullptr};

  int last_char = ' ';
  std::string identifier_str;
  double num_val{0};

  int cur_tok{0};

  Token cur_type{tok_dt};

  std::vector<std::unique_ptr<Expr>> exprs;
};

}  // namespace smcc
