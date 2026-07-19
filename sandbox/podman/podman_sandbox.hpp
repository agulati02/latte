#pragma once

#include <sandbox/sandbox.hpp>

namespace sandbox {

class PodmanSandbox : public Sandbox
{
public:
    ~PodmanSandbox() override = default;

    std::unique_ptr<SandboxProcess> run(const SandboxConfig& config) override;
};

}
