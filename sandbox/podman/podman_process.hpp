#pragma once

#include <sandbox/sandbox_process.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/process.hpp>

namespace bp = boost::process::v2;

namespace sandbox {

class PodmanProcess : public SandboxProcess {

public:

    // io_ctx must be kept alive for the lifetime of proc
    explicit PodmanProcess(
        std::unique_ptr<boost::asio::io_context> io_ctx,
        std::unique_ptr<bp::process> proc
    ) : io_ctx_(std::move(io_ctx)), proc_(std::move(proc)) {}

    void write(std::string_view) override;

    bool readLine(std::string&) override;

    int wait() override;

    void terminate() override;

private:

    std::unique_ptr<boost::asio::io_context> io_ctx_;
    std::unique_ptr<bp::process> proc_;

};

}
