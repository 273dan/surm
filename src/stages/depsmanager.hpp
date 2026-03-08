#pragma once
#include "surmfile/surmfile.hpp"
#include "log.hpp"
#include <filesystem>
#include <stdexcept>
namespace surm {
class DepsManager {
public:
  DepsManager(const SurmFile& validated_surmfile) : 
    validated_surmfile_(validated_surmfile),
    absolute_path_to_dotsurm(validated_surmfile.absolute_path_to_parent_folder / ".surm"),
    absolute_path_to_base_dep_dir(validated_surmfile.absolute_path_to_parent_folder / ".surm" / "deps"),
    absolute_path_to_base_include_dir(validated_surmfile.absolute_path_to_parent_folder / ".surm" / "include") {}
  void fetch(bool force = false) {
    if(!validated_surmfile_.deps.has_value() or validated_surmfile_.deps->deps.empty()) {
      log::Logger::log_immediate(log::NoDepsToFetch{});
      return;
    }
    log::DepsFetched log_message;
    
    for(auto& dep: validated_surmfile_.deps->deps) {
      if(!dep_exists_locally(dep)) {
        download_to_deps(dep);
        link_to_include(dep);
        log_message.fetched++;
      }
      else {
        log_message.skipped++;
      }
    }
    log::Logger::log_immediate(log_message);
  }

  std::filesystem::path get_path_to_include_dir(const section::Dep& dep) {
    return absolute_path_to_base_include_dir
        / dep.v_author.value()
        / dep.v_repo.value()
        / "";
  }
  void clean() {
    if(dep_dir_exists()) std::filesystem::remove_all(absolute_path_to_base_dep_dir);
    if(include_dir_exists()) std::filesystem::remove_all(absolute_path_to_base_include_dir);
  }


private:
  const SurmFile& validated_surmfile_;
  const std::filesystem::path absolute_path_to_dotsurm;
  const std::filesystem::path absolute_path_to_base_dep_dir;
  const std::filesystem::path absolute_path_to_base_include_dir;

  bool dep_dir_exists() const {
    return std::filesystem::exists(absolute_path_to_base_dep_dir);
  }
  bool include_dir_exists() const {
    return std::filesystem::exists(absolute_path_to_base_include_dir);
  }
  bool dep_exists_locally(const section::Dep& dep) {
    return std::filesystem::exists(get_path_to_file_in_deps(dep));

  }

  std::string get_dep_file_url(const section::Dep& dep) {
    if(dep.release.value() == "latest") {
      throw std::runtime_error("latest tag not supported");
    }
    return std::format(
        "https://github.com/{}/{}/releases/download/v{}/{}",
        dep.v_author.value(),
        dep.v_repo.value(),
        dep.release.value(),
        dep.file.value()
      );
  }
  void download_to_deps(const section::Dep& dep) {
    const auto dep_dir = get_path_to_dep_dir(dep);
    if(!std::filesystem::exists(dep_dir)) {
      std::filesystem::create_directories(dep_dir);
    }
    auto url = get_dep_file_url(dep);
    auto output_path = get_path_to_file_in_deps(dep);
    std::string command = std::format(
        "curl -sL {} -o {}",
        url,
        output_path.string()
    );
    std::system(command.c_str());
    if(!std::filesystem::exists(output_path)) {
      throw std::runtime_error("file failed to download");
    }
  }
  void link_to_include(const section::Dep& dep) {
    auto to = get_path_to_file_in_deps(dep);
    auto from = get_path_to_file_in_include(dep);
    auto include_dir = get_path_to_include_dir(dep);
    if(!std::filesystem::exists(include_dir)) {
      std::filesystem::create_directories(include_dir);
    }
    std::filesystem::create_symlink(to, from);
  }
  /* Filesystem helpers */
  std::filesystem::path get_path_to_dep_dir(const section::Dep& dep) {
    return absolute_path_to_base_dep_dir
        / dep.v_author.value()
        / dep.v_repo.value()
        / dep.release.value()
        / "";
  }
  std::filesystem::path get_path_to_file_in_deps(const section::Dep& dep) {
    return get_path_to_dep_dir(dep) / dep.file.value();
  }
  std::filesystem::path get_path_to_file_in_include(const section::Dep& dep) {
    return get_path_to_include_dir(dep) / dep.file.value();
       
  }

};
}
