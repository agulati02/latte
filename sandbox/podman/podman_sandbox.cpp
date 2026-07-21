#include <sandbox/podman/podman_sandbox.hpp>
#include <sandbox/podman/podman_process.hpp>

#include <spdlog/spdlog.h>
#include <boost/process.hpp>
#include <boost/process/v2/environment.hpp>
#include <boost/asio/io_context.hpp>

namespace asio = boost::asio;
namespace bp = boost::process::v2;
namespace bpenv = bp::environment;

namespace sandbox {

std::unique_ptr<SandboxProcess> PodmanSandbox::run(const SandboxConfig& config)
{
    spdlog::info("[PodmanSandbox::run] Launching container image '{}'", config.image);

    auto podman_exe = bpenv::find_executable("podman");
    if (podman_exe.empty()) {
        throw std::runtime_error("podman executable not found in PATH");
    }

    std::vector<std::string> args;
    args.push_back("run");
    args.push_back("--rm");

    if (!config.network_enabled) {
        args.push_back("--network=none");
    }

    for (const auto& [key, val] : config.env) {
        args.push_back("--env");
        args.push_back(key + "=" + val);
    }

    for (const auto& mount : config.mounts) {
        std::string flag = mount.host_path.string() + ":" + mount.sandbox_path.string();
        flag += (mount.mode == MountMode::ReadOnly) ? ":ro" : ":rw";
        args.push_back("-v");
        args.push_back(std::move(flag));
    }

    args.push_back(config.image);

    for (const auto& arg : config.command) {
        args.push_back(arg);
    }

    auto io_ctx = std::make_unique<asio::io_context>();
    auto proc = std::make_unique<bp::process>(*io_ctx, podman_exe, args);

    return std::make_unique<PodmanProcess>(std::move(io_ctx), std::move(proc));
}

}
