# Requirements Document

## Introduction

This milestone wires the full `latte grind` pipeline end-to-end so that the roaster Python process (`roaster/main.py`) runs inside an isolated Podman container and its JSON progress events are streamed back to the C++ caller.

The DI chain is already scaffolded (`CLI → ModelGrinder → ImporterClient → Sandbox → PodmanSandbox`) but every layer below `ImporterClient` is a stub. This spec covers what must be built to make `latte grind <model>` actually launch the roaster in Podman, read its stdout, and report progress to the terminal.

## Requirements

### Requirement 1: Roaster container image is buildable and runnable

**User Story:** As a developer running `latte grind`, I want the roaster Python script to be packaged as a Podman image, so that the C++ runtime can launch it in an isolated environment.

#### Acceptance Criteria

1. WHEN the developer runs `podman build -t latte-roaster ./roaster` THE SYSTEM SHALL complete the build without error.
2. WHEN the developer runs `podman run --rm latte-roaster` THE SYSTEM SHALL emit the following newline-delimited JSON to stdout in order and exit 0: `{"type":"started"}`, four `{"type":"progress","value":N}` lines (N = 25, 50, 75, 100), and `{"type":"completed","artifact":"qwen.latte"}`.
3. WHEN the C++ code references the roaster image by name THE SYSTEM SHALL use the same image tag that was built in criterion 1.

---

### Requirement 2: `PodmanSandbox::run()` spawns a real container process

**User Story:** As a component that holds a `Sandbox` reference, I want `PodmanSandbox::run(config)` to translate `SandboxConfig` into a live Podman process, so that callers can read its stdout and wait for it to finish.

#### Acceptance Criteria

1. WHEN `run()` is called THE SYSTEM SHALL construct a `podman run` argument vector without any shell string construction.
2. WHEN `config.network_enabled` is `false` THE SYSTEM SHALL include `--network=none` in the argument vector.
3. WHEN `config.read_only` is `true` THE SYSTEM SHALL include `--read-only` in the argument vector.
4. WHEN `config.mounts` contains entries THE SYSTEM SHALL translate each `Mount` to a `-v host:sandbox:ro` or `-v host:sandbox:rw` flag according to `MountMode`.
5. WHEN `config.env` contains entries THE SYSTEM SHALL translate each entry to a `-e KEY=VALUE` flag.
6. WHEN `config.memory_limit` is set THE SYSTEM SHALL include `--memory=<value>` in the argument vector.
7. WHEN `config.cpu_limit` is set THE SYSTEM SHALL include `--cpus=<value>` in the argument vector.
8. WHEN a container is launched THE SYSTEM SHALL always pass `--rm` so containers are cleaned up after exit.
9. WHEN the process starts successfully THE SYSTEM SHALL return a non-null `std::unique_ptr<SandboxProcess>` wrapping the live child.
10. WHEN Podman fails to start THE SYSTEM SHALL throw a descriptive `std::runtime_error` rather than returning `nullptr`.

---

### Requirement 3: A concrete `SandboxProcess` implementation exists for Podman

**User Story:** As a caller of `SandboxProcess`, I want a concrete implementation backed by a Boost.Process child, so that I can read stdout line-by-line, write to stdin, wait for exit, and terminate the process.

#### Acceptance Criteria

1. WHEN `readLine(std::string& out)` is called and a line is available THE SYSTEM SHALL populate `out` and return `true`.
2. WHEN `readLine(std::string& out)` is called and the process has closed stdout THE SYSTEM SHALL return `false`.
3. WHEN `write(std::string_view data)` is called THE SYSTEM SHALL write `data` to the child process's stdin.
4. WHEN `wait()` is called THE SYSTEM SHALL block until the child exits and return its integer exit code.
5. WHEN `terminate()` is called THE SYSTEM SHALL forcibly terminate the child process.
6. WHEN the public header for `PodmanProcess` is included THE SYSTEM SHALL not expose any Boost.Process types in that header.

---

### Requirement 4: `ImporterClient::fetch()` builds a correct `SandboxConfig` and streams events

**User Story:** As a consumer of the compilation pipeline, I want `ImporterClient::fetch(model_id)` to correctly configure and launch the roaster sandbox then stream its JSON events, so that progress is visible and the artifact name is captured.

#### Acceptance Criteria

1. WHEN `fetch()` builds a `SandboxConfig` THE SYSTEM SHALL set `image` to `"latte-roaster"` (not an artifact filename).
2. WHEN `fetch()` builds a `SandboxConfig` THE SYSTEM SHALL set `network_enabled = false` and `read_only = true`.
3. WHEN `m_sandbox->run(config)` returns a `SandboxProcess` THE SYSTEM SHALL enter a `readLine()` loop until EOF.
4. WHEN a line containing `{"type":"started"}` is read THE SYSTEM SHALL log a start message.
5. WHEN a line containing `{"type":"progress","value":N}` is read THE SYSTEM SHALL log the progress value.
6. WHEN a line containing `{"type":"completed","artifact":"<name>"}` is read THE SYSTEM SHALL log the artifact name.
7. WHEN the read loop ends THE SYSTEM SHALL call `process->wait()` and log an error if the exit code is non-zero.
8. WHEN a line cannot be parsed as valid JSON THE SYSTEM SHALL log a warning and continue without crashing.

---

### Requirement 5: CMake build graph is complete and correct

**User Story:** As a developer building the project, I want all CMake targets to declare their dependencies correctly, so that the project links without errors after the new code is added.

#### Acceptance Criteria

1. WHEN `latte_grind` is linked THE SYSTEM SHALL include `latte_sandbox` as a dependency so sandbox types and the `PodmanSandbox` implementation resolve without linker errors.
2. WHEN `latte_sandbox` is built THE SYSTEM SHALL compile both `podman/podman_sandbox.cpp` and `podman/podman_process.cpp` as part of the target.
3. WHEN the developer runs `cmake --build build` THE SYSTEM SHALL complete the full chain `latte → latte_cli → latte_grind → latte_sandbox` without errors or warnings.

---

### Requirement 6: End-to-end `latte grind` command works

**User Story:** As a user running the CLI, I want `./latte grind <model-name>` to launch the roaster container and print its progress events, so that I can observe the compilation pipeline working.

#### Acceptance Criteria

1. WHEN the user runs `./latte grind <any-model-name>` THE SYSTEM SHALL print a started message, four progress messages (25, 50, 75, 100), and a completed message with artifact `qwen.latte`, in that order.
2. WHEN the roaster exits with code 0 THE SYSTEM SHALL exit with code 0.
3. WHEN the command finishes THE SYSTEM SHALL leave no dangling Podman containers behind.
