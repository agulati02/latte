#pragma once

#include <string>
#include <sandbox/sandbox.hpp>

namespace compiler {

class ImporterClient {
public:
    explicit ImporterClient(
        std::unique_ptr<sandbox::Sandbox> sandbox
    ) : m_sandbox(std::move(sandbox)) {}

    void fetch(const std::string& model_id);

private:
    std::unique_ptr<sandbox::Sandbox> m_sandbox;
};


}
