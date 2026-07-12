#include <fmt/format.h>
#include <string>

int main() {

    int port = 8080;

    std::string msg = fmt::format("Listening on port {}", port);

    fmt::print("{}\n", msg);

    return 0;
}