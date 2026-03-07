#pragma once
#include "section_executable.hpp"
#include "surmfile.hpp"
#include "section_project.hpp"
#include "autodetection.hpp"
#include <optional>
namespace surm {
class Validator {
public:
  Validator(const SurmFile& original) : original_surmfile_{original}, new_surmfile_{original}, ad_{original.path} {}

  [[nodiscard]]
  std::optional<SurmFile> validate() {
    validate_project();
    validate_executable();


    return is_valid ? std::optional<SurmFile>{new_surmfile_} : std::nullopt;
    return new_surmfile_;
  }
private:
  void validate_project() {
    new_surmfile_.project = new_surmfile_.project.value_or(section::Project{});
    auto& proj = new_surmfile_.project.value();
    proj.name = proj.name.value_or("project");
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
    }
    else {
      if(!ad_.check_files_exist(exec.sources)) {
        is_valid = false;
      }
    }
  }

  SurmFile new_surmfile_;
  Autodetection ad_;
  const SurmFile& original_surmfile_;

  /* Validator state */
  bool is_valid                  = true;

};
}
