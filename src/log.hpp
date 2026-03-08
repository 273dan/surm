#pragma once
#include <variant>
#include <vector>
#include <string>
#include <print>
#include "log_utils.hpp"
namespace surm::log {
enum Level {
  Info,
  Warn,
  Error
};
struct GenericMessage{ Level level; std::string message; };
struct MissingSourcesForTarget { std::string target; std::vector<std::string> sources; };
struct DepMissingRepo { };
struct DepRepoMalformed { std::string repo; };
struct DepsFetched { size_t fetched{}; size_t skipped{}; };


class Logger {
public:
  using LogMessage = std::variant<GenericMessage,
                                  MissingSourcesForTarget,
                                  DepMissingRepo,
                                  DepRepoMalformed,
                                  DepsFetched>;
  static Logger& get() {
    static Logger logger;
    return logger;
  }
  static void add_message(LogMessage message) {
    Logger& logger = Logger::get();
    logger.messages.push_back(message);
  }

  static void flush_messages() {
    Logger& logger = Logger::get();
    for(const auto& message: logger.messages) {
      log_immediate(message);
    }
    logger.messages.clear();
  }
  static void log_immediate(LogMessage message) {
    std::visit([](const auto& msg) { log(msg); }, message);
  }
  std::vector<LogMessage> messages{};
  static void log(const GenericMessage& message) {
    switch(message.level) {
      case Level::Info:
        log::info("{}", message.message);
        break;
      case Level::Warn:
        log::warn("{}", message.message);
        break;
      case Level::Error:
        log::error("{}", message.message);
        break;
    }
  }
  static void log(const MissingSourcesForTarget& message) {
    log::error("missing sources for target {}:", message.target);
    for(auto& source: message.sources) {
      std::println("    - {}", source);
    }
  }
  static void log(const DepMissingRepo& message) {
    log::error("dependency with missing 'repo'");
  }
  static void log(const DepRepoMalformed& message) {
    log::error("repo string {} is malformed", message.repo);
  }
  static void log(const DepsFetched& message) {
    std::cout
      << rang::style::bold
      << rang::fg::blue
      << "[i] "
      << rang::style::reset
      << "fetched "
      << rang::fg::blue
      << message.fetched
      << rang::fg::reset
      << " dependencies";
    if(message.skipped == 0) {
      std::cout << "\n";
      return;
    }
    std::cout
      << " ("
      << rang::fg::yellow
      << message.skipped
      << rang::style::reset
      << " skipped)\n";
  }
};
}
