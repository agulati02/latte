#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>
#include <filesystem>

namespace sandbox {

enum class MountMode {
    ReadOnly,
    ReadWrite
};

struct Mount {
    std::filesystem::path host_path;
    std::filesystem::path sandbox_path;
    MountMode mode = MountMode::ReadOnly;
};

struct SandboxConfig
{
    std::string image;

    std::vector<std::string> command;

    std::map<std::string, std::string> env;

    std::vector<Mount> mounts;

    bool network_enabled = false;

    bool read_only = true;

    std::optional<size_t> memory_limit;

    std::optional<int> cpu_limit;
};

}