#include <sandbox/podman/podman_sandbox.hpp>

#include <spdlog/spdlog.h>

namespace sandbox {

std::unique_ptr<SandboxProcess> PodmanSandbox::run(const SandboxConfig& config)
{
    // TODO: spawn a podman container using config.image and config.command
    spdlog::info("[PodmanSandbox::run] Launching container image '{}'", config.image);
    return nullptr;
}

}
