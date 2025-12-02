#include <gtest.h>
#include "stack.h"

TEST(TStack, can_create_stack_with_positive_size) {
  ASSERT_NO_THROW(TStack<int> s(10));
}

TEST(TStack, throws_when_creating_stack_with_non_positive_size) {
  ASSERT_ANY_THROW(TStack<int> s(0));
  ASSERT_ANY_THROW(TStack<int> s(-5));
}

TEST(TStack, new_stack_is_empty_and_count_is_zero) {
  TStack<int> s(5);
  EXPECT_TRUE(s.is_empty());
  EXPECT_EQ(0, s.get_count());
}

TEST(TStack, can_push_and_pop) {
  TStack<int> s(5);
  s.push(10);
  s.push(20);

  EXPECT_EQ(2, s.get_count());
  EXPECT_EQ(20, s.pop());
  EXPECT_EQ(1, s.get_count());
  EXPECT_EQ(10, s.pop());
  EXPECT_TRUE(s.is_empty());
}

TEST(TStack, can_check_top_element) {
  TStack<int> s(5);
  s.push(42);
  EXPECT_EQ(42, s.top());
  EXPECT_EQ(1, s.get_count());
  s.pop();
  EXPECT_TRUE(s.is_empty());
}

TEST(TStack, stack_resizes_when_capacity_is_exceeded) {
  TStack<int> s(3);
  s.push(1);
  s.push(2);
  s.push(3);

  ASSERT_NO_THROW(s.push(4));

  s.push(5);
  s.push(6);
  EXPECT_EQ(6, s.get_count());
  EXPECT_EQ(6, s.pop());
  EXPECT_EQ(5, s.pop());
  EXPECT_EQ(4, s.pop());
}

TEST(TStack, throws_when_pop_from_empty) {
  TStack<int> s(2);
  ASSERT_THROW(s.pop(), std::underflow_error);
}

TEST(TStack, throws_when_top_from_empty) {
  TStack<int> s(2);
  ASSERT_THROW(s.top(), std::underflow_error);
}

TEST(TStack, can_clear_stack) {
  TStack<int> s(5);
  s.push(1);
  s.push(2);
  s.clear();
  EXPECT_TRUE(s.is_empty());
  EXPECT_EQ(0, s.get_count());
}
