#include <string>
#include <commands/grind.hpp>
#include <compiler/grind.hpp>

void register_grind(CLI::App& app) {
    struct Options {
        std::string model_name;
    };

    auto opts = std::make_shared<Options>();

    auto* cmd = app.add_subcommand("grind", "Load and parse a model from Hugging Face");
    cmd->add_option("model_name", opts->model_name, "Model identifier; used to fetch from HuggingFace")->required();

    cmd->callback([opts]() {
        // Delegate execution to grind module
        auto model_grinder = std::make_unique<grind::ModelGrinder>(); 
        model_grinder->compile(opts->model_name);
    });
}
