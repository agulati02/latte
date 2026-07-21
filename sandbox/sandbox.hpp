#pragma once

#include <memory>
#include <sandbox/sandbox_config.hpp>
#include <sandbox/sandbox_process.hpp>

namespace sandbox {

class Sandbox
{
public:

    virtual ~Sandbox() = default;

    virtual std::unique_ptr<SandboxProcess> run(
        const SandboxConfig& config
    ) = 0;
};

}
