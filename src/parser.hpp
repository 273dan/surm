#pragma once
#include <string>
#include "surmfile.hpp"
#include <yaml-cpp/yaml.h>
namespace surm {
class Parser {
public:
  Parser(const std::string& path) : yaml_file_{YAML::LoadFile(path)}, absolute_path_{path} {}
  SurmFile get_surmfile() {
    return SurmFile::from_yaml(absolute_path_, yaml_file_);
  }
private:
  YAML::Node yaml_file_;
  std::filesystem::path absolute_path_;

};
}
