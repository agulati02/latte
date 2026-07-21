#include <string>
#include <commands/grind.hpp>
#include <compiler/grind.hpp>
#include <compiler/importer_client.hpp>
#include <sandbox/sandbox_factory.hpp>

void register_grind(CLI::App& app) {
    struct Options {
        std::string model_name;
    };

    auto opts = std::make_shared<Options>();

    auto* cmd = app.add_subcommand("grind", "Load and parse a model from Hugging Face");
    cmd->add_option("model_name", opts->model_name, "Model identifier; used to fetch from HuggingFace")->required();

    cmd->callback([opts]() {
        auto importer = std::make_unique<compiler::ImporterClient>(
            sandbox::SandboxFactory::create()
        );
        grind::ModelGrinder grinder(std::move(importer));
        grinder.compile(opts->model_name);
    });
}
