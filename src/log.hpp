#pragma once
#include "rang.hpp"
#include <format>
#include <iostream>
namespace surm::log {
class Logger {
public:
  template <typename ...Args>
  static void error(std::format_string<Args...> fmt, Args&&... args) {
    std::cout
      << rang::style::bold
      << rang::fg::red
      << "[!] "
      << std::format(fmt, std::forward<Args>(args)...) 
      << rang::style::reset
      << "\n";
  }
  template <typename ...Args>
  static void warn(std::format_string<Args...> fmt, Args&&... args) {
    std::cout
      << rang::style::bold
      << rang::fg::yellow
      << "[?] "
      << std::format(fmt, std::forward<Args>(args)...) 
      << rang::style::reset
      << "\n";
  }
  static Logger& get() {
    static Logger logger;
    return logger;
  }
};

}
