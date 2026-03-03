#include <yaml-cpp/yaml.h>
#include <rang.hpp>
#include <indicators/progress_spinner.hpp>
#include <filesystem>
#include <format>
#include "surmfile.hpp"
#include "log.hpp"

int main(int argc, const char** argv) {
  if (!std::filesystem::exists("./surm.yaml")) {
    Log::error("No surm.yaml file found");
    return 1;
  }
  YAML::Node surmyaml{YAML::LoadFile("./surm.yaml")};
  std::optional<SurmFile> surmfile{SurmFile::from_yaml(surmyaml)};
  if (!surmfile.has_value()) {
    Log::error("Unable to parse surm.yaml file");
    return 1;
  }
  if(argc <= 1) {
    return 0;
  }
  if (std::string(argv[1]) == "build") {
    std::system(std::vformat("g++ {} -o {}", std::make_format_args(surmfile->executable->sources_as_str, surmfile->executable->name.value())).c_str());
  }
  return 0;
}
