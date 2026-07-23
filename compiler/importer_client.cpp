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
        .image = "latte-roaster:latest",
        .env = std::map<std::string, std::string>{
            {"MODEL_ID", model_id}
        }
    };

    int exit_code = m_sandbox->run(config)->wait();

    spdlog::info("[ImporterClient::fetch] Model grind process exited with status code {}", exit_code);
}

}
