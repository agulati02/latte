# LattE Architecture

## High-Level Flow

### `latte grind` — Model Compilation

```
CLI (core/main.cpp)
 │
 ▼
latte_cli  (cli/)
 │  CLI11 subcommand dispatch
 ▼
register_grind  (cli/commands/grind.cpp)
 │  calls grind::compile(model_name)
 ▼
grind::compile  (compiler/grind.cpp)
 │  TODO: invoke ImporterClient
 ▼
ImporterClient::fetch  (compiler/importer_client.cpp)
 │  TODO: inject Sandbox, build SandboxConfig
 ▼
Sandbox (interface)  (sandbox/sandbox.hpp)
 │
 ├──────────────┐
 ▼              ▼
PodmanSandbox  DockerSandbox  ...
 │  (sandbox/podman/)
 ▼
Isolated Process
 │  stdin/stdout as newline-delimited JSON
 ▼
latte-roaster  (roaster/main.py)
 │
 ▼
model.latte
```

### `latte serve` — Model Inference

`latte serve` is **not yet implemented**. The intended flow once the inference runtime exists:

```
CLI
 │
 ▼
Core runtime  (core/)
 │  Load model.latte
 ▼
LattE IR executor
 │
 ▼
Inference result
```

Python is never involved in the serve path.

---

## CMake Targets

| Target | Type | Directory | Links to |
|---|---|---|---|
| `latte` | executable | `core/` | `latte_cli` |
| `latte_cli` | static lib | `cli/` | `latte_grind`, `CLI11` |
| `latte_grind` | static lib | `compiler/` | `cpr`, `nlohmann_json` |
| `latte_sandbox` | static lib | `sandbox/` | `Boost::process` |

All targets inherit `project_options` (C++20, warnings-as-errors, `fmt`, `spdlog`).

> Note: `latte_sandbox` is not yet linked into the CLI chain. It will be wired in when `ImporterClient` is refactored to accept `std::unique_ptr<Sandbox>`.

---

## Component Responsibilities

### CLI (`cli/`)
- Parse command-line arguments via CLI11
- Dispatch to subcommand handlers (`register_grind`, future `register_serve`)
- Display progress and report errors
- Must not know about Podman, Docker, or process execution

### Grind (`compiler/grind.*`) — namespace `grind`
- Entry point for the compilation pipeline, called `grind::compile(model_name)`
- Will coordinate `ImporterClient` to launch the roaster and stream progress
- Does not speak directly to the sandbox or any container runtime

### ImporterClient (`compiler/importer_client.*`) — namespace `compiler`
- Abstract interface: `virtual void fetch(const std::string& model_id) = 0`
- Concrete implementation: `ImportClient` (currently a stub in the same `.cpp`)
- Will own: building `SandboxConfig`, injecting `Sandbox`, streaming JSON events from the roaster back to the caller
- Depends only on the `Sandbox` interface — never on `PodmanSandbox` directly

### Sandbox (`sandbox/`) — namespace `sandbox`
- Abstract interface: `Sandbox::run(SandboxConfig) → unique_ptr<SandboxProcess>`
- Manages process lifecycle, stdin/stdout/stderr, filesystem mounts, network and resource isolation
- Must not contain any knowledge of Hugging Face, models, Python, grind, or serve

### Sandbox Backends
- `PodmanSandbox` (`sandbox/podman/`) — stub, logs image name, returns `nullptr`
- `DockerSandbox` — not yet created
- `BubblewrapSandbox` — future
- `SandboxFactory` — not yet created; will select backend at runtime

### Roaster (`roaster/`)
- Standalone Python application (`roaster/main.py`), containerised via `roaster/Dockerfile` (python:3.12-slim)
- Current implementation: stub that emits fake progress events and a `qwen.latte` artifact name
- Communicates over stdout as **newline-delimited JSON**
- Event schema (current):
  - `{"type": "started"}`
  - `{"type": "progress", "value": <0–100>}`
  - `{"type": "completed", "artifact": "<filename>.latte"}`
- Completely unaware of Podman or any container runtime

### Core (`core/`)
- Currently only `main.cpp` — calls `run()` from `latte_cli`
- Intended future role: C++ inference runtime for `latte serve`
- Will load and execute `.latte` artifacts with no Python dependency

---

## Key Interfaces

```cpp
// sandbox/sandbox.hpp
class Sandbox {
public:
    virtual ~Sandbox() = default;
    virtual std::unique_ptr<SandboxProcess> run(const SandboxConfig&) = 0;
};

// sandbox/sandbox_process.hpp
class SandboxProcess {
public:
    virtual ~SandboxProcess() = default;
    virtual void write(std::string_view) = 0;
    virtual bool readLine(std::string&) = 0;
    virtual int wait() = 0;
    virtual void terminate() = 0;
};
```

### SandboxConfig and Mount
`Mount` and `MountMode` live directly in `sandbox/sandbox_config.hpp` (no separate `mount.hpp`):

```cpp
// sandbox/sandbox_config.hpp — namespace sandbox
enum class MountMode { ReadOnly, ReadWrite };

struct Mount {
    std::filesystem::path host_path;
    std::filesystem::path sandbox_path;
    MountMode mode = MountMode::ReadOnly;
};

struct SandboxConfig {
    std::string image;
    std::vector<std::string> command;
    std::map<std::string, std::string> env;
    std::vector<Mount> mounts;
    bool network_enabled = false;
    bool read_only = true;
    std::optional<size_t> memory_limit;
    std::optional<int> cpu_limit;
};
```

Backend translation: Podman → `-v host:container:ro` · Docker → `--mount` · Bubblewrap → `--ro-bind`

---

## Project Layout (actual)

```
latte/
├── cli/
│   ├── latte_cli.hpp / latte_cli.cpp   # run() entry point, CLI11 app
│   └── commands/
│       └── grind.hpp / grind.cpp       # register_grind()
├── compiler/
│   ├── grind.hpp / grind.cpp           # grind::compile() — namespace grind
│   └── importer_client.hpp / .cpp      # ImporterClient interface + ImportClient stub
├── sandbox/
│   ├── sandbox.hpp                     # Sandbox interface
│   ├── sandbox_config.hpp              # SandboxConfig, Mount, MountMode
│   ├── sandbox_process.hpp             # SandboxProcess interface
│   └── podman/
│       └── podman_sandbox.hpp / .cpp   # PodmanSandbox (stub)
├── core/
│   └── main.cpp                        # binary entry point
├── roaster/
│   ├── main.py                         # Python stub emitting JSON events
│   └── Dockerfile
└── cmake/
    ├── Dependencies.cmake
    └── ProjectOptionsInterface.cmake
```

---

## Dependency Injection

Always inject via `std::unique_ptr`. Never slice.

```cpp
// Correct
auto sandbox = SandboxFactory::create();
ImporterClient client(std::move(sandbox));

// Wrong — slices the derived object
Base b = Derived();
```

---

## Process Execution

Never build shell command strings. Always use an executable + argument vector via Boost.Process:

```cpp
// Correct
cfg.executable = "podman";
cfg.arguments = { "run", "--rm", "--network=none", image };

// Wrong
std::string cmd = "podman run " + image;
system(cmd.c_str());
```

---

## Podman Integration

There is no official C++ Podman SDK. Invoke the Podman CLI via Boost.Process using an argument vector. The REST API is available but adds unnecessary complexity for current needs.

---

## Design Principles

- **Single responsibility** — each component owns exactly one concern
- **Dependency inversion** — depend on `Sandbox` interface, not on `PodmanSandbox`
- **Backend-agnostic sandbox** — `SandboxConfig` contains no backend-specific fields
- **No shell string building** — always use executable + argv
- **Reusable sandbox library** — `latte_sandbox` is an independent CMake target
- **Protocol independence** — the importer protocol (JSON events) evolves separately from the sandbox

---

## Immediate Implementation Goals

1. Implement `SandboxFactory` in `sandbox/` to select backend at runtime
2. Add `ProcessRunner` wrapper around Boost.Process in `sandbox/process/`
3. Implement `PodmanSandbox::run()` — translate `SandboxConfig` into a real `podman run` invocation
4. Refactor `ImporterClient` / `ImportClient` to accept `std::unique_ptr<Sandbox>` via constructor and build `SandboxConfig` from the model id
5. Wire `grind::compile()` to instantiate and call `ImporterClient`, streaming roaster JSON events to the terminal
6. Link `latte_sandbox` into `latte_grind` (or `latte_cli`) once `ImporterClient` depends on it
7. Implement `latte serve` subcommand and `core/` inference runtime
