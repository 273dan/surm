#include <filesystem>
#include <vector>
#include <ranges>
namespace surm {

class Autodetection {
public:
  Autodetection(const std::filesystem::path& path) : root_dir_{path.parent_path()} {};
  std::vector<std::string> autodetect_sources_for_single_exec() {

    namespace fs = std::filesystem;
    auto it = fs::recursive_directory_iterator{root_dir_};

    return std::ranges::subrange(it, fs::recursive_directory_iterator{})
        | std::views::filter([](const fs::directory_entry& dir_entry) -> bool {
            if (!dir_entry.is_regular_file()) return false;
            auto ext = dir_entry.path().extension();
            return ext == ".cpp" || ext == ".hpp";
        })
        | std::views::transform([](const fs::directory_entry& dir_entry) {
            return dir_entry.path().string();
        })
        | std::ranges::to<std::vector<std::string>>();
  }

  bool check_files_exist(const std::vector<std::string>& files_rel_to_root) {
    for(const auto& path: files_rel_to_root) {
      auto abs_path = root_dir_.append(path);
      if(!std::filesystem::exists(abs_path)) {
        return false;
      }
    }
    return true;
  }

private:
  std::filesystem::path root_dir_;

};
}
