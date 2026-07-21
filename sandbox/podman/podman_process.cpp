#include <sandbox/podman/podman_process.hpp>

namespace sandbox {

void PodmanProcess::write(std::string_view) {}

bool PodmanProcess::readLine(std::string&) { return true; }

int PodmanProcess::wait() { return proc_->wait(); }

void PodmanProcess::terminate() {}

}
