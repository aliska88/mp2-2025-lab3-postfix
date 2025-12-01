#ifndef POSTFIX_H
#define POSTFIX_H

#include <string>
#include "lexemes.h"
#include "stack.h"

class Postfix {
 public:
  Postfix();
  ~Postfix();

  void set_infix(std::string infix);
  std::string get_postfix_string();

  void parse();
  void to_postfix();
  double calculate();

 private:
  std::string infix_;

  Token** infixTokens_;
  int infixLen_;
  int infixMaxLen_;

  Token** postfixTokens_;
  int postfixLen_;
  int postfixMaxLen_;

  void add_token(Token**& arr, int& len, int& maxLen, Token* item);

  void clean();
};

#endif
