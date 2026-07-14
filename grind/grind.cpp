#include <grind/grind.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace grind {

void execute(std::string& model_name) {
    spdlog::info("Grinding {} beans", model_name);
}

}
