#include "util.hpp"
#include "surmfile.hpp"
namespace surm {

class Builder {
public:
  Builder(const SurmFile& surmfile) : surmfile_{surmfile} {}
  int build() const {
    const auto& exec = surmfile_.executable.value();
    const auto& standard =  surmfile_.project->standard;
    const std::string sources{util::space_sep_vec_str(exec.sources)};
    const std::string cmd = std::format(
        "g++ {} -o {} --std=c++{}",
        sources,
        exec.name.value(),
        standard.value()
    );
    int ec = std::system(cmd.c_str());
    return ec;
  }
private:
  SurmFile surmfile_;

};
}
