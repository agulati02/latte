#pragma once

#include <string>
#include <string_view>

namespace sandbox {

class SandboxProcess
{
public:

    virtual ~SandboxProcess() = default;

    virtual void write(std::string_view) = 0;

    virtual bool readLine(std::string&) = 0;

    virtual int wait() = 0;

    virtual void terminate() = 0;
};

}


