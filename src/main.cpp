#include <yaml-cpp/yaml.h>
#include <rang.hpp>
#include <indicators/progress_spinner.hpp>
#include <filesystem>
#include <format>
#include "surmfile.hpp"
#include "log.hpp"
#include "CLI11.hpp"

int main(int argc, const char** argv) {
  CLI::App app{"surm"};
  app.add_subcommand("show", "Print surm.yaml as surm sees it")
    ->alias("s");
  app.add_subcommand("show-auto", "Print surm.yaml populated with auto-discovered items (WIP description)")
    ->alias("sa");
  CLI11_PARSE(app, argc, argv);
  if (!std::filesystem::exists("./surm.yaml")) {
    Log::error("No surm.yaml file found");
    return 1;
  }
  YAML::Node surmyaml{YAML::LoadFile("./surm.yaml")};
  std::optional<SurmFile> surmfile{SurmFile::from_yaml(surmyaml)};
  if(app.parsed()) {
    if(app.get_subcommand("show")->parsed()) {
      surmfile->print_debug();
    }
  }

  return 0;
}
