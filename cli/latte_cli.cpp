#include <latte_cli.hpp>
#include <CLI/CLI.hpp>
#include <commands/grind.hpp>

int run(int argc, char **argv) {

    CLI::App app("LattE - LLM Runtime Environment");
    app.require_subcommand();

    // register subcommands
    register_grind(app);

    CLI11_PARSE(app, argc, argv);
    return 0;

}
