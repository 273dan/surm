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
  std::optional<std::pair<std::string, std::string>> split_repo_by_slash(const std::string& repo) {
    auto slash_pos = repo.find('/');
    if(slash_pos == std::string::npos)
      return std::nullopt;
    std::string left = repo.substr(0, slash_pos);
    std::string right = repo.substr(slash_pos + 1);
    return std::make_optional(std::pair(left, right));
  }

}
}
