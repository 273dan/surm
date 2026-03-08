#pragma once
#include "log.hpp"
#include "section_executable.hpp"
#include "surmfile.hpp"
#include "section_project.hpp"
#include "autodetection.hpp"
#include "util.hpp"
#include <optional>
namespace surm {
class Validator {
public:
  Validator(const SurmFile& original) : original_surmfile_{original}, new_surmfile_{original}, ad_{original.absolute_path_to_file} {}

  [[nodiscard]]
  std::optional<SurmFile> validate() {
    validate_project();
    validate_executable();
    validate_deps();


    return is_valid ? std::make_optional(new_surmfile_) : std::nullopt;
    return new_surmfile_;
  }
private:
  void validate_project() {
    new_surmfile_.project = new_surmfile_.project.value_or(section::Project{});
    auto& proj = new_surmfile_.project.value();
    proj.name = proj.name.value_or(new_surmfile_.absolute_path_to_file.parent_path().filename());
    proj.version = proj.version.value_or("0.0.0");
    proj.standard = proj.standard.value_or("17");
    
  }
  void validate_executable() {
    if(!new_surmfile_.executable.has_value()) {
      new_surmfile_.executable = section::Executable{};
    }
    auto& exec = new_surmfile_.executable.value();
    if(!exec.name.has_value()) {
      exec.name = "main";
    }
    if(exec.sources.empty()) {
      exec.sources = ad_.autodetect_sources_for_single_exec();
      if(exec.sources.empty()) {
        log::Logger::add_message(log::NoSources{});

          is_valid = false;
        
      }
    }
    else {
      const auto missing_files = ad_.check_files_exist(exec.sources);
      if(!missing_files.empty()) {
        log::Logger::add_message(log::MissingSourcesForTarget{
            .target = exec.name.value(),
            .sources = missing_files
        });
        is_valid = false;
      }
    }
  }
  void validate_deps() {
    if(!new_surmfile_.deps.has_value()) {
      return;
    }
    for(auto& dep: new_surmfile_.deps->deps) {
      if(!dep.repo.has_value()) {
        is_valid = false;
        log::Logger::add_message(log::DepMissingRepo{});
        continue;
      }
      auto author_repo = util::split_repo_by_slash(dep.repo.value());
      if (!author_repo.has_value()) {
        is_valid = false;
        log::Logger::add_message(log::DepRepoMalformed{dep.repo.value()});
      }
      dep.v_author = author_repo->first;
      dep.v_repo = author_repo->second;
      


    }

  }

  SurmFile new_surmfile_;
  Autodetection ad_;
  const SurmFile& original_surmfile_;

  /* Validator state */
  bool is_valid                  = true;

};
}
