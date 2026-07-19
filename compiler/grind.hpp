#pragma once

#include <string>
#include <sandbox/sandbox_factory.hpp>

#include "importer_client.hpp"

namespace grind {

class ModelGrinder {

public:
    explicit ModelGrinder()
        : m_importer(
            std::make_unique<compiler::ImporterClient>(
                sandbox::SandboxFactory::create()
            )
        ) {}

    // Source-agnostic driver function
    void compile(std::string& model_name);

private:
    std::unique_ptr<compiler::ImporterClient> m_importer;

};

}
