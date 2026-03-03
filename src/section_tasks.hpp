#pragma once
#include "yaml-cpp/node/node.h"
#include <optional>
#include <string>
#include <unordered_map>
struct TaskItem {
  std::string script;
};
struct Tasks {
  static std::optional<Tasks> from_yaml(const YAML::Node& node) {
    return std::nullopt;

  }
  std::unordered_map<std::string, TaskItem> tasks;
};

