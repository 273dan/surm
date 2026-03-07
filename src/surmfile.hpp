#pragma once
#include "section_deps.hpp"
#include "section_project.hpp"
#include "section_tasks.hpp"
#include "section_executable.hpp"
#include "section_targets.hpp"
#include "yaml-cpp/node/node.h"
#include <optional>
#include <filesystem>
namespace surm {

struct SurmFile {
  static SurmFile from_yaml(const std::filesystem::path& path, const YAML::Node& node) {
      SurmFile file {
        .project = section::Project::from_yaml(node),
        .tasks = section::Tasks::from_yaml(node),
        .deps = section::Deps::from_yaml(node),
        .executable = section::Executable::from_yaml(node),
        .targets = section::Targets::from_yaml(node),
        .path = path
      };
      return file;
  }
  void print_debug() const {
    if(project.has_value()) project->print_debug();
    if(tasks.has_value()) tasks->print_debug();
    if(deps.has_value()) deps->print_debug();
    if(executable.has_value()) executable->print_debug();
    if(targets.has_value()) targets->print_debug();

  }
  std::optional<section::Project> project;
  std::optional<section::Tasks> tasks;
  std::optional<section::Deps> deps;
  std::optional<section::Executable> executable;
  std::optional<section::Targets> targets;
  std::filesystem::path path;
};
}
