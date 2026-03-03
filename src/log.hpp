#pragma once
#include "rang.hpp"
#include <format>
#include <iostream>
namespace Log {
  template <typename ...Args>
  void error(std::format_string<Args...> fmt, Args&&... args) {
    std::cout
      << rang::style::bold
      << rang::fg::red
      << "[!] "
      << std::format(fmt, std::forward<Args>(args)...) 
      << rang::style::reset
      << "\n";
  }
  template <typename ...Args>
  void warn(std::format_string<Args...> fmt, Args&&... args) {
    std::cout
      << rang::style::bold
      << rang::fg::yellow
      << "[?] "
      << std::format(fmt, std::forward<Args>(args)...) 
      << rang::style::reset
      << "\n";
  }

}
