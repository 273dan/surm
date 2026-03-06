#include "yaml-cpp/node/node.h"
#include <optional>
#include <print>
struct Deps {
  static std::optional<Deps> from_yaml(const YAML::Node& yaml) {
    return std::nullopt;
  }
  void print_debug() const {
    std::println("deps:");
    for(auto& dep: deps)
      std::println("  {}", dep);
  }
  std::vector<std::string> deps;

};
