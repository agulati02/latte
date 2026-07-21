#pragma once

#include <sandbox/sandbox.hpp>
#include <sandbox/podman/podman_sandbox.hpp>

namespace sandbox {

class SandboxFactory {

public:
    static std::unique_ptr<Sandbox> create();

};

}
