#include <compiler/grind.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace grind {

void ModelGrinder::compile(std::string& model_name) {
    spdlog::info("[ModelGrinder::compile] Grinding {} beans", model_name);

    m_importer->fetch(model_name);
}

}
