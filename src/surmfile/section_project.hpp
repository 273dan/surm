#pragma once
#include "util.hpp"
#include "yaml-cpp/node/node.h"
#include <optional>
#include <print>
#include <string>
namespace surm::section {
struct Project {
  using ostr = std::optional<std::string>;
  static std::optional<Project> from_yaml(const YAML::Node& node) {
    auto& node_proj = node["project"];
    if(!node_proj.IsDefined()) {
      return std::nullopt;
    }
    return Project {
      .name = util::value_or_nullopt<std::string>("name", node_proj),
      .version = util::value_or_nullopt<std::string>("version", node_proj),
      .standard = util::value_or_nullopt<std::string>("standard", node_proj),
    };
  }
  void print_debug() const {
    std::println("project:");
    std::println("  name: {}", name.value_or(""));
    std::println("  version: {}", version.value_or(""));
    std::println("  standard: {}", standard.value_or(""));

  }
  ostr name;
  ostr version;
  ostr standard;
};
}
