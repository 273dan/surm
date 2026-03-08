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

class Logger {
public:
  using LogMessage = std::variant<GenericMessage,
                                  MissingSourcesForTarget,
                                  DepMissingRepo,
                                  DepRepoMalformed>;
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
      std::visit([](const auto& msg) { log(msg); }, message);
    }
    logger.messages.clear();
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
};
}
