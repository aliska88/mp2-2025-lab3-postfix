#include <gtest.h>
#include "arithmetic.h"

TEST(Postfix, can_calculate_simple_addition) {
  Postfix p;
  p.set_infix("2 + 2");
  p.parse();
  p.to_postfix();
  EXPECT_DOUBLE_EQ(4.0, p.calculate());
}

TEST(Postfix, respects_operations_priority) {
  Postfix p;
  p.set_infix("2 + 2 * 3");
  p.parse();
  p.to_postfix();
  EXPECT_DOUBLE_EQ(8.0, p.calculate());
}

TEST(Postfix, respects_parentheses) {
  Postfix p;
  p.set_infix("(2 + 2) * 3");
  p.parse();
  p.to_postfix();
  EXPECT_DOUBLE_EQ(12.0, p.calculate());
}

TEST(Postfix, handles_nested_parentheses) {
  Postfix p;
  p.set_infix("((10 - 2) / 4) + 1");
  p.parse();
  p.to_postfix();
  EXPECT_DOUBLE_EQ(3.0, p.calculate());
}

TEST(Postfix, handles_floating_point_numbers) {
  Postfix p;
  p.set_infix("1.5 * 2.5 + 1.25");
  p.parse();
  p.to_postfix();
  EXPECT_DOUBLE_EQ(5.0, p.calculate());
}

TEST(Postfix, handles_multidigit_numbers) {
  Postfix p;
  p.set_infix("100 - 25 * 3");
  p.parse();
  p.to_postfix();
  EXPECT_DOUBLE_EQ(25.0, p.calculate());
}

TEST(Postfix, converts_simple_expression_correctly) {
  Postfix p;
  p.set_infix("1 + 2 * 3");
  p.parse();
  p.to_postfix();
  std::string expected = "1.000000 2.000000 3.000000 * + ";
  EXPECT_EQ(expected.size(), p.get_postfix_string().size());
}

TEST(Postfix, converts_with_parentheses_correctly) {
  Postfix p;
  p.set_infix("(10 + 5) / 3");
  p.parse();
  p.to_postfix();
  std::string expected = "10.000000 5.000000 + 3.000000 / ";
  EXPECT_EQ(expected.size(), p.get_postfix_string().size());
}

TEST(Postfix, throws_on_unbalanced_parentheses_missing_closing) {
  Postfix p;
  p.set_infix("(2 + 2");
  p.parse();
  ASSERT_ANY_THROW(p.to_postfix());
}

TEST(Postfix, throws_on_unbalanced_parentheses_missing_opening) {
  Postfix p;
  p.set_infix("2 + 2)");
  p.parse();
  ASSERT_ANY_THROW(p.to_postfix());
}

TEST(Postfix, throws_on_unknown_symbol_during_parse) {
  Postfix p;
  p.set_infix("2 $ 3");
  ASSERT_ANY_THROW(p.parse());
}

TEST(Postfix, throws_on_double_dot_during_parse) {
  Postfix p;
  p.set_infix("2.2.5 + 1");
  ASSERT_ANY_THROW(p.parse());
}

TEST(Postfix, throws_on_division_by_zero) {
  Postfix p;
  p.set_infix("5 / 0");
  p.parse();
  p.to_postfix();
  ASSERT_ANY_THROW(p.calculate());
}

TEST(Postfix, throws_on_too_few_operands) {
  Postfix p;
  p.set_infix("2 +");
  p.parse();
  p.to_postfix();
  ASSERT_ANY_THROW(p.calculate());
}

TEST(Postfix, throws_on_too_many_operands) {
  Postfix p;
  p.set_infix("2 + 3 4");
  p.parse();
  p.to_postfix();
  ASSERT_ANY_THROW(p.calculate());
}
