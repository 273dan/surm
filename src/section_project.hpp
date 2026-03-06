#include "yaml-cpp/node/node.h"
#include <optional>
#include <print>
#include <string>
struct Project {
  using ostr = std::optional<std::string>;
  static std::optional<Project> from_yaml(const YAML::Node& node) {
    auto& node_proj = node["project"];
    return Project {
      .name = node_proj["name"] ? ostr{node_proj["name"].as<std::string>()} : std::nullopt,
      .version = node_proj["version"] ? ostr{node_proj["version"].as<std::string>()} : std::nullopt,
    };
  }
  void print_debug() const {
    std::println("project:");
    std::println("  name: {}", name.value_or(""));
    std::println("  version: {}", version.value_or(""));

  }
  ostr name;
  ostr version;
};
