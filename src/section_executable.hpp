#pragma once
#include "log.hpp"
#include "yaml-cpp/node/node.h"
#include <optional>
#include <ranges>
#include <string>
#include <vector>
#include <print>
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
      Log::error("No sources specified for executable {}", executable.name.value_or(""));
      return std::nullopt;
    }
    executable.sources = node_executable["sources"].as<std::vector<std::string>>();
    executable.sources_as_str = std::ranges::to<std::string>( executable.sources | std::views::join_with(' '));
    return executable;

  }
  void print_debug() const {
    std::println("executable");
    std::println("  name: {}", name.value_or(""));
    std::println("  sources:");
    for(auto& source: sources) {
      std::println("    {}", source);
    }

  }
  std::optional<std::string> name{};
  std::vector<std::string> sources{};
  std::string sources_as_str;
};
