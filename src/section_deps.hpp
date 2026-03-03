#include "yaml-cpp/node/node.h"
#include <optional>
struct Deps {
  static std::optional<Deps> from_yaml(const YAML::Node& yaml) {
    return std::nullopt;
  }

};
