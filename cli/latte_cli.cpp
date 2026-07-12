#include <latte_cli.hpp>
#include <CLI/CLI.hpp>

int run(int argc, char **argv) {

    CLI::App app;

    // CLI parsing happens here

    CLI11_PARSE(app, argc, argv);

    return 0;

}
