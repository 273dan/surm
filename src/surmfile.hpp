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
  std::optional<Project> project;
  std::optional<Tasks> tasks;
  std::optional<Deps> deps;
  std::optional<Executable> executable;
};
