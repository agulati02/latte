#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <sandbox/sandbox.hpp>

#include <compiler/importer_client.hpp>

namespace compiler {

void ImporterClient::fetch(const std::string& model_id)
{
    // TODO: orchestrate model fetching and sandbox invocation
    spdlog::info("[ImporterClient::fetch] Fetching model_id = {}", model_id);
    
    sandbox::SandboxConfig config {
        .image = "qwen-qwen3-0-6b.latte"
    };
    
    m_sandbox->run(config);
}

}
