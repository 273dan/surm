#include "section_deps.hpp"
#include "section_project.hpp"
#include "section_tasks.hpp"
#include "section_executable.hpp"
#include "yaml-cpp/node/node.h"
#include <optional>
struct SurmFile {
  static std::optional<SurmFile> from_yaml(const YAML::Node& node) {
      SurmFile file {
        .project = Project::from_yaml(node),
        .tasks = Tasks::from_yaml(node),
        .deps = Deps::from_yaml(node),
        .executable = Executable::from_yaml(node),
      };
      return file;
  }
  void print_debug() const {
    if(project.has_value()) project->print_debug();
    if(tasks.has_value()) tasks->print_debug();
    if(deps.has_value()) deps->print_debug();
    if(executable.has_value()) executable->print_debug();

  }
  std::optional<Project> project;
  std::optional<Tasks> tasks;
  std::optional<Deps> deps;
  std::optional<Executable> executable;
};
