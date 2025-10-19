#include "prelimitary.hpp"
#include <iostream>
#include <sstream>
#include <string>

char opening(char c) {
  switch (c) {
    case ')': return '(';
    case ']': return '[';
    case '}': return '{';
    default: return '\0';
  }
}

int get_precedence(char c) {
  switch (c) {
    case '+': case '-':
      return 1;
    case '*': case '/':
      return 2;
    case '(':
      return 3;
    default:
      return 0;
  }
}

std::string to_rpn(const std::string& expression) {
  LinkedStack<char> operations;
  std::stringstream result;

  for (char c : expression) {
    if (c == '(') {
      operations.push(c);
    } else if (c >= '0' && c <= '9') {
      result << c;
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
      while (
        !operations.empty() &&
        operations.peek() != '(' &&
        get_precedence(operations.peek()) > get_precedence(c)
      ) {
        result << operations.pop();
      }

      operations.push(c);
    } else if (c == ')') {
      while (!operations.empty() && operations.peek() != '(') {
        result << operations.pop();
      }

      operations.pop();
    }
  }

  while (!operations.empty()) {
    result << operations.pop();
  }

  return result.str();
}

bool balanced(const std::string& expression) {
  LinkedStack<char> brackets;

  for (char c : expression) {
    if (c == '(' || c == '[' || c == '{') {
      brackets.push(c);
    } else if (c == ')' || c == ']' || c == '}') {
      if (brackets.peek() != opening(c)) {
        return false;
      }

      brackets.pop();
    }
  }

  return brackets.empty();
}

int main() {
  std::string expression;
  std::getline(std::cin, expression);

  std::cout << std::boolalpha << balanced(expression) << '\n';
  // std::cout << to_rpn(expression) << '\n';

  return 0;
}