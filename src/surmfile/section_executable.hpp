#pragma once
#include "yaml-cpp/node/node.h"
#include <optional>
#include <string>
#include <vector>
#include <print>
namespace surm::section {
struct Executable {
  static std::optional<Executable> from_yaml(const YAML::Node& node) {
    Executable executable{};
    if(!node["executable"]) {
      return std::nullopt;
    }
    auto& node_executable = node["executable"];
    if(node_executable["name"]) {
      executable.name = node_executable["name"].as<std::string>();
    }
    if(!node["executable"]["sources"]) {
      return std::nullopt;
    }
    if(node_executable["sources"].IsSequence()) {
      executable.sources = node_executable["sources"].as<std::vector<std::string>>();
    }
    else {
      executable.sources = {node_executable["sources"].as<std::string>()};
    }
    return executable;

  }
  void print_debug() const {
    std::println("executable:");
    std::println("  name: {}", name.value_or(""));
    std::println("  sources:");
    for(auto& source: sources) {
      std::println("    {}", source);
    }

  }
  std::optional<std::string> name{};
  std::vector<std::string> sources{};
};
}
