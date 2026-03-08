#include <yaml-cpp/yaml.h>
#include <rang.hpp>
#include <indicators/progress_spinner.hpp>
#include <filesystem>
#include <format>
#include "surmfile.hpp"
#include "validator.hpp"
#include "builder.hpp"
#include "log.hpp"
#include "CLI11.hpp"
#include "parser.hpp"
#include "depsmanager.hpp"

int main(int argc, const char** argv) {
  CLI::App app{"surm"};
  app.require_subcommand();
  app.add_subcommand("show", "Print surm.yaml as surm sees it")
    ->alias("s");
  app.add_subcommand("which", "Print the full path of the current surm.yaml file")
    ->alias("w");
  app.add_subcommand("show-auto", "Print surm.yaml populated with auto-discovered items (WIP description)")
    ->alias("sa");
  app.add_subcommand("build", "Build the project")
    ->alias("b");
  app.add_subcommand("run", "Build + run the project")
    ->alias("r");
  app.add_subcommand("fetch", "Fetch dependencies")
    ->alias("f");
  app.add_subcommand("clean", "Remove all local dependencies")
    ->alias("c");
  CLI11_PARSE(app, argc, argv);
  std::filesystem::path surmfile_path_absolute{std::filesystem::absolute("surm.yaml")};
  if (!std::filesystem::exists(surmfile_path_absolute)) {
    surm::log::error("No surm.yaml file found");
    return 1;
  }
  surm::Parser parser{surmfile_path_absolute};
  auto user_surmfile = parser.get_surmfile();
  surm::log::Logger::flush_messages();
  /* Surmfile parsed */
  if(app.parsed()) {
    if(app.get_subcommand("show")->parsed()) {
      user_surmfile.print_debug();
      return 0;
    }
    if(app.get_subcommand("which")->parsed()) {
      std::println("{}", user_surmfile.absolute_path_to_file.string());
      return 0;
    }
  }


  /* Surmfile validated */
  surm::Validator validator{user_surmfile};
  std::optional<surm::SurmFile> validated_surmfile_opt =  validator.validate();
  surm::log::Logger::flush_messages();
  if(!validated_surmfile_opt.has_value()) {
    surm::log::failure("build failed");
    return 1;
  }


  auto validated_surmfile = std::move(validated_surmfile_opt.value());
  if(app.get_subcommand("show-auto")->parsed()) {
    validated_surmfile.print_debug();
    return 0;
  }
  /* DepsManager created */
  surm::DepsManager depsmanager{validated_surmfile};
  if(app.got_subcommand("fetch")) {
    depsmanager.fetch();
  }
  if(app.got_subcommand("clean")) {
    depsmanager.clean();
  }

  /* Builder created */
  surm::Builder builder{validated_surmfile, depsmanager};
  if(app.get_subcommand("build")->parsed()) {
    int ec = builder.build();
  }

  return 0;
}


