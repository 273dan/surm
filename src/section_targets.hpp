#pragma once
#include "yaml-cpp/node/node.h"
#include <print>
#include "util.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
namespace surm::section {
struct Target {
  enum TargetType {
    Executable,
    Library
  };
  static inline std::optional<std::string> ttype_opt_to_opt_str(std::optional<section::Target::TargetType> target_type) {
    return target_type.transform([](auto& type){
        switch(type) {
          case section::Target::Executable:
            return "executable";
          case section::Target::Library:
            return "library";
        }
    });
  }
  std::optional<std::string> name;
  std::optional<TargetType> target_type;
  std::optional<std::vector<std::string>> sources;
};
struct Targets {
  static std::optional<Targets> from_yaml(const YAML::Node& node) {
    Targets targets{};
    if(!node["targets"]) {
      return std::nullopt;
    }
    auto& node_targets = node["targets"];
    auto vec_target_nodes = node_targets.as<std::vector<YAML::Node>>();
    for(auto& node: vec_target_nodes) {
      std::optional<Target::TargetType> target_type;
      if (node["type"]) {
        auto type_str = node["type"].as<std::string>();
        if(type_str == "executable") target_type = Target::Executable;
        else if(type_str == "library") target_type = Target::Library;
        else target_type = std::nullopt;
      }
      else target_type = std::nullopt;
        

      Target target{
        .name = util::value_or_nullopt<std::string>("name", node),
        .target_type = target_type,
        .sources = util::value_or_nullopt<std::vector<std::string>>("sources", node),

      };
      targets.targets.push_back(target);
    }
    return targets;
    


  }
  void print_debug() const {
    std::println("targets:");
    for(auto& target: targets) {
      std::println("  name: {}", target.name.value_or(""));
      std::println("  type: {}", Target::ttype_opt_to_opt_str(target.target_type).value_or(""));
      std::println("  sources:");
      if(target.sources.has_value()) {
        for(auto& source: target.sources.value()) {
          std::println("    {}", source);
        }
      }
    }


  }
  std::unordered_map<std::string, Target> targets_by_name;
  std::vector<Target> targets;

};

}
