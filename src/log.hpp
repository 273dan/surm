#pragma once
#include "rang.hpp"
#include <format>
#include <iostream>
#include <vector>
namespace surm::log {
struct LogMessage {
  enum Level {
    Info,
    Warn,
    Error
  };
  Level level;
  std::string message;

};
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
  static Logger& get() {
    static Logger logger;
    return logger;
  }
  template <typename ...Args>
  static void add_message(LogMessage::Level level, std::format_string<Args...> fmt, Args&&... args) {
    Logger& logger = Logger::get();
    logger.messages.push_back(LogMessage{
        .level = level,
        .message = std::format(fmt, std::forward<Args>(args)...),
    });
  }
  static void flush_messages() {
    Logger& logger = Logger::get();
    for(auto& message: logger.messages) {
      switch(message.level) {
        case LogMessage::Error:
          error("{}", message.message);
          break;
        case LogMessage::Warn:
          warn("{}", message.message);
          break;
        case LogMessage::Info:
          info("{}", message.message);
          break;
      }

    }
    logger.messages.clear();


  }
  std::vector<LogMessage> messages{};

};

}
