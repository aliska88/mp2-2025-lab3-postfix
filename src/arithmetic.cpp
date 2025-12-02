#include <cctype>
#include <iostream>
#include <string>
#include "lexemes.h"
#include "arithmetic.h"

Postfix::Postfix() {
  infixTokens_ = nullptr;
  infixLen_ = 0;
  infixMaxLen_ = 0;

  postfixTokens_ = nullptr;
  postfixLen_ = 0;
  postfixMaxLen_ = 0;
}

Postfix::~Postfix() { clean(); }

void Postfix::clean() {
  if (infixTokens_ != nullptr) {
    for (int i = 0; i < infixLen_; i++) delete infixTokens_[i];
    delete[] infixTokens_;
  }
  delete[] postfixTokens_;

  infixTokens_ = nullptr;
  postfixTokens_ = nullptr;
  infixLen_ = infixMaxLen_ = postfixLen_ = postfixMaxLen_ = 0;
}

void Postfix::add_token(Token**& arr, int& len, int& maxLen, Token* item) {
  if (len >= maxLen) {
    int newMaxLen = (maxLen == 0) ? 10 : maxLen * 2;
    Token** newArr = new Token*[newMaxLen];

    for (int i = 0; i < len; ++i) newArr[i] = arr[i];

    if (arr != nullptr) delete[] arr;

    arr = newArr;
    maxLen = newMaxLen;
  }

  arr[len++] = item;
}

void Postfix::set_infix(std::string infix) {
  clean();
  infix_ = infix;
}

void Postfix::parse() {
  for (size_t i = 0; i < infix_.length(); i++) {
    char c = infix_[i];

    if (isspace(c)) continue;

    if (isdigit(c) || c == '.') {
      std::string numStr;
      bool hasDot = false;
      while (i < infix_.length() && (isdigit(infix_[i]) || infix_[i] == '.')) {
        if (infix_[i] == '.') {
          if (hasDot)
            throw std::runtime_error("Error at pos " + std::to_string(i) +
                                     ": double dot");
          hasDot = true;
        }
        numStr += infix_[i];
        i++;
      }
      i--;

      add_token(infixTokens_, infixLen_, infixMaxLen_,
               new Operand(std::stod(numStr)));
    } else if (Operation::is_expr_operator(c)) {
      add_token(infixTokens_, infixLen_, infixMaxLen_, new Operation(c));
    } else {
      throw std::runtime_error("Error at pos " + std::to_string(i) +
                               ": unknown symbol");
    }
  }
}

void Postfix::to_postfix() {
  if (postfixTokens_ != nullptr) delete[] postfixTokens_;
  postfixTokens_ = nullptr;
  postfixLen_ = 0;
  postfixMaxLen_ = 0;

  TStack<Operation*> opStack(10);

  for (int i = 0; i < infixLen_; i++) {
    Token* t = infixTokens_[i];

    if (t->get_type() == TOKEN_OPERAND) {
      add_token(postfixTokens_, postfixLen_, postfixMaxLen_, t);
    } else if (t->get_type() == TOKEN_OPERATION) {
      Operation* op = dynamic_cast<Operation*>(t);
      char sym = op->get_symbol();

      if (sym == '(') {
        opStack.push(op);
      } else if (sym == ')') {
        bool found = false;
        while (!opStack.is_empty()) {
          Operation* topOp = opStack.pop();
          if (topOp->get_symbol() == '(') {
            found = true;
            break;
          }
          add_token(postfixTokens_, postfixLen_, postfixMaxLen_, topOp);
        }
        if (!found)
          throw std::runtime_error("Error: unbalanced parentheses missing (");
      } else {
        while (!opStack.is_empty()) {
          Operation* topOp = opStack.top();
          if (topOp->get_symbol() == '(') break;

          if (topOp->get_priority() >= op->get_priority()) {
            add_token(postfixTokens_, postfixLen_, postfixMaxLen_,
                     opStack.pop());
          } else {
            break;
          }
        }

        opStack.push(op);
      }
    }
  }

  while (!opStack.is_empty()) {
    Operation* topOp = opStack.pop();
    if (topOp->get_symbol() == '(')
      throw std::runtime_error("Error: unbalanced parentheses missing )");
    add_token(postfixTokens_, postfixLen_, postfixMaxLen_, topOp);
  }
}

double Postfix::calculate() {
  TStack<double> calcStack(10);

  for (int i = 0; i < postfixLen_; i++) {
    Token* t = postfixTokens_[i];

    if (t->get_type() == TOKEN_OPERAND) {
      Operand* op = dynamic_cast<Operand*>(t);
      calcStack.push(op->get_value());
    } else {
      Operation* op = dynamic_cast<Operation*>(t);

      if (calcStack.is_empty()) throw std::runtime_error("Not enough operands");
      double v2 = calcStack.pop();

      if (calcStack.is_empty()) throw std::runtime_error("Not enough operands");
      double v1 = calcStack.pop();

      double res = 0;
      switch (op->get_symbol()) {
        case '+':
          res = v1 + v2;
          break;
        case '-':
          res = v1 - v2;
          break;
        case '*':
          res = v1 * v2;
          break;
        case '/':
          if (v2 == 0) throw std::runtime_error("Division by zero");
          res = v1 / v2;
          break;
      }
      calcStack.push(res);
    }
  }

  if (calcStack.is_empty()) return 0;
  double result = calcStack.pop();
  if (!calcStack.is_empty()) throw std::runtime_error("Too many operands");

  return result;
}

std::string Postfix::get_postfix_string() {
  std::string s = "";
  for (int i = 0; i < postfixLen_; i++) {
    if (postfixTokens_[i]->get_type() == TOKEN_OPERAND) {
      s += std::to_string(
               dynamic_cast<Operand*>(postfixTokens_[i])->get_value()) +
           " ";
    } else {
      s += std::string(
               1, dynamic_cast<Operation*>(postfixTokens_[i])->get_symbol()) +
           " ";
    }
  }
  return s;
}
