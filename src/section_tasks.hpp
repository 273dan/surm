#pragma once
#include "yaml-cpp/node/node.h"
#include <optional>
#include <print>
#include <string>
#include <unordered_map>
namespace surm::section {
struct TaskItem {
  std::string script;
  std::vector<std::string> script_lines;
};
struct Tasks {
  static std::optional<Tasks> from_yaml(const YAML::Node& node) {
    return std::nullopt;
  }
  void print_debug() const {
    std::println("tasks:");
    for(auto& [name, task]: tasks) {
      std::println("  {}:", name);
      for(auto& line: task.script_lines) {
        println("    {}", line);
      }
    }
  }
  std::unordered_map<std::string, TaskItem> tasks;
};
}
