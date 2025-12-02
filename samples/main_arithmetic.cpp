#include <iostream>

#include "arithmetic.h"

int main() {
  Postfix calculator;
  std::string expr;

  std::cout << "Enter expression: ";
  std::getline(std::cin, expr);

  try {
    calculator.set_infix(expr);
    calculator.parse();
    calculator.to_postfix();

    std::cout << "Postfix: " << calculator.get_postfix_string() << std::endl;
    std::cout << "Result: " << calculator.calculate() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
