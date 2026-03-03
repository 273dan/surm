#include "yaml-cpp/node/node.h"
#include <optional>
#include <string>
#include "log.hpp"
struct Project {
  static std::optional<Project> from_yaml(const YAML::Node& node) {
    if(!node["project"]) {
      Log::warn("Missing 'project' section. See `surm --help project`");
      return std::nullopt;
    }
    return std::nullopt;
    
  }
  std::string name;
  std::string version;
};
