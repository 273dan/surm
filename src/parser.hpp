#pragma once
#include <string>
#include "surmfile.hpp"
#include <yaml-cpp/yaml.h>
namespace surm {
class Parser {
public:
  Parser(const std::string& path) : yaml_file_{YAML::LoadFile(path)}, path_{path} {}
  SurmFile get_surmfile() {
    return SurmFile::from_yaml(path_, yaml_file_);
  }
private:
  YAML::Node yaml_file_;
  std::filesystem::path path_;

};
}
