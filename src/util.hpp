#pragma once
#include "yaml-cpp/node/node.h"
#include <optional>
#include <ranges>
namespace surm::util {
namespace {


  template <typename T>
    std::optional<T> value_or_nullopt(const std::string& key, const YAML::Node& node) {
      return node[key].IsDefined() ? std::optional<T>{node[key].as<T>()} : std::nullopt;
    }

  std::string space_sep_vec_str(const std::vector<std::string>& input) {
    return std::ranges::to<std::string>( input | std::views::join_with(' '));
  }

}
}
