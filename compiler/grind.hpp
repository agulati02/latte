#pragma once

#include <memory>
#include <string>
#include <compiler/importer_client.hpp>

namespace grind {

class ModelGrinder {

public:
    explicit ModelGrinder(std::unique_ptr<compiler::ImporterClient> importer)
        : m_importer(std::move(importer)) {}

    // Source-agnostic driver function
    void compile(std::string& model_name);

private:
    std::unique_ptr<compiler::ImporterClient> m_importer;

};

}
