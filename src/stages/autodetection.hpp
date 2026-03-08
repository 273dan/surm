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
        | std::views::filter([this](const fs::directory_entry& dir_entry) -> bool {
            std::filesystem::path deps_dir = root_dir_ / ".surm" / "deps";
            auto [it_deps_dir, it_current_path] = std::mismatch(deps_dir.begin(), deps_dir.end(), dir_entry.path().begin(), dir_entry.path().end());
            return it_deps_dir != deps_dir.end();
            
        })
        | std::views::transform([this](const fs::directory_entry& dir_entry) {
            return dir_entry.path().lexically_relative(root_dir_).string();
        })
        | std::ranges::to<std::vector<std::string>>();
  }

  std::vector<std::string> check_files_exist(const std::vector<std::string>& files_rel_to_root) {
    return files_rel_to_root
      | std::views::filter([this](auto& file){
          return !(std::filesystem::exists(root_dir_ / file));
        })
      | std::ranges::to<std::vector<std::string>>();
  }

private:
  const std::filesystem::path root_dir_;

};
}
