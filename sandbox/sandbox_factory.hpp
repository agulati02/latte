#pragma once

#include "sandbox.hpp"
#include "podman/podman_sandbox.hpp"

namespace sandbox {

class SandboxFactory {

public:
    static std::unique_ptr<Sandbox> create();

};

}
