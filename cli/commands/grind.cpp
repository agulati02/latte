#include <commands/grind.hpp>

void register_grind(CLI::App& app) {
    struct Options {
        std::string model_name;
    };

    auto opts = std::make_shared<Options>();

    auto* grind = app.add_subcommand("grind", "Load and parse a model from Hugging Face");
    grind->add_option("model_name", opts->model_name, "Model identifier")->required();

    grind->callback([opts]() {
        // opts->model_name is valid here
    });
}
