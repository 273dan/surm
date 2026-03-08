#pragma once
#include <format>
#include <iostream>
#include <rang.hpp>
namespace surm::log {
namespace {
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
  template <typename ...Args>
  static void info(std::format_string<Args...> fmt, Args&&... args) {
    std::cout
      << rang::style::bold
      << rang::fg::blue
      << "[i] "
      << rang::style::reset
      << std::format(fmt, std::forward<Args>(args)...) 
      << "\n";
  }
  template <typename ...Args>
  static void failure(std::format_string<Args...> fmt, Args&&... args) {
    std::cout
      << rang::style::bold
      << rang::fgB::gray
      << "[x] "
      << std::format(fmt, std::forward<Args>(args)...) 
      << "\n";
  }




}
}
