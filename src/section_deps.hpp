#include "yaml-cpp/node/node.h"
#include <optional>
#include <print>
#include "util.hpp"
namespace surm::section {
struct Dep {
  std::optional<std::string> repo;
  std::optional<std::string> release;
  std::optional<std::string> file;
  std::optional<std::string> v_author;
  std::optional<std::string> v_repo;
};
struct Deps {
  static std::optional<Deps> from_yaml(const YAML::Node& node) {
    Deps deps{};
    if(!node["deps"])
      return std::nullopt;
    auto vec_dep_nodes = node["deps"].as<std::vector<YAML::Node>>();
    for(auto& node: vec_dep_nodes) {
      Dep dep{
        .repo = util::value_or_nullopt<std::string>("repo", node),
        .release = util::value_or_nullopt<std::string>("release", node),
        .file = util::value_or_nullopt<std::string>("file", node),
      };
      deps.deps.push_back(dep);
    }
    return deps;
  }
  void print_debug() const {
    std::println("deps:");
    for(auto& dep: deps) {
      std::println("  repo: {}", dep.repo.value_or(""));
      std::println("    release: {}", dep.release.value_or(""));
      std::println("    file: {}", dep.file.value_or(""));
    }
  }
  std::vector<Dep> deps;

};
}
