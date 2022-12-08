#include "../common/filereader.h"
#include <unordered_map>
#include <memory>
#include <set>

struct Dir {
    std::string name = "";
    std::vector<uint32_t> files;
    std::vector<std::shared_ptr<Dir>> sub_dirs;
    std::shared_ptr<Dir> parent;
};

void split_line(std::string& line, std::vector<std::string>& collector) {
    collector.clear();
    std::stringstream ss(line);
    std::string temp_str;
    while (ss >> temp_str) collector.push_back(temp_str);
}

uint64_t count_size(std::shared_ptr<Dir> pwd, std::vector<uint64_t>& size_vec) {
    uint64_t size = 0;
    for (auto& file_size : pwd->files)
        size += file_size;
    for (auto& dir : pwd->sub_dirs)
        size += count_size(dir, size_vec);
    size_vec.push_back(size);
    return size;
}

int main() {
    auto lines = aoc_read_lines_raw();
    uint64_t small_folder_sum = 0;
    uint64_t free_space = 70000000;
    std::vector<uint64_t> size_vec;
    std::shared_ptr<Dir> pwd = std::make_shared<Dir>();
    std::unordered_map<std::string, std::shared_ptr<Dir>> dir_map;
    std::set<std::string> file_map;
    dir_map["/"] = std::make_shared<Dir>();
    dir_map["/"]->name = "/";
    std::vector<std::string> line;
    for (size_t i = 0; i < lines.size(); i++) {
        if (!lines[i].compare("$ ls")) continue;
        if (!lines[i].compare("$ cd ..")) {
            pwd = pwd->parent;
            continue;
        }
        split_line(lines[i], line);
        if (line[1] == "cd")
            pwd = dir_map[pwd->name + line[2]];
        else if (line[0] == "dir") {
            std::string dir_name = pwd->name + line[1];
            if (dir_map.find(dir_name) == dir_map.end()) {
                std::shared_ptr<Dir> new_dir = std::make_shared<Dir>();
                new_dir->name = dir_name;
                new_dir->parent = pwd;
                dir_map[dir_name] = new_dir;
                pwd->sub_dirs.push_back(new_dir);
            }
        } else {
            std::string file_name = pwd->name + line[1];
            if (file_map.find(file_name) == file_map.end()) {
                file_map.insert(file_name);
                pwd->files.push_back(std::stoi(line[0]));
            }
        }
    }

    free_space -= count_size(dir_map["/"], size_vec);
    uint64_t minimal_dir = 30000000;
    uint64_t sub_100K_sum = 0;
    for (auto& file_size : size_vec) {
        if (file_size <= 100000)
            sub_100K_sum += file_size;
        if (free_space + file_size > 30000000 && file_size < minimal_dir)
            minimal_dir = file_size;
    }
    std::cout << sub_100K_sum << std::endl;
    std::cout << minimal_dir << std::endl;


}