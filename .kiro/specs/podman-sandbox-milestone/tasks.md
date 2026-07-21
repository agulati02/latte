# Implementation Plan

- [ ] 1. Verify and fix the roaster container image
  - Review `roaster/Dockerfile` and confirm it has a correct `CMD ["python", "main.py"]` entrypoint
  - Run `podman build -t latte-roaster ./roaster` and resolve any build errors
  - Run `podman run --rm latte-roaster` and confirm all six JSON event lines appear on stdout in the correct order
  - Note the exact image tag string `"latte-roaster"` — it will be used as a constant in task 4
  - _Requirements: 1_

- [ ] 2. Implement `PodmanProcess` (concrete `SandboxProcess`)
  - Create `sandbox/podman/podman_process.hpp` declaring `class PodmanProcess : public SandboxProcess` in namespace `sandbox` with private members for the Boost.Process child and stdout pipe stream; keep all Boost headers out of the public header
  - Create `sandbox/podman/podman_process.cpp` and implement `readLine(std::string& out)` using `std::getline` on the stdout pipe, returning `true` on success and `false` on EOF or error
  - Implement `write(std::string_view data)` to write to the child's stdin pipe and flush
  - Implement `wait()` to call `m_child.wait()` and return `m_child.exit_code()`
  - Implement `terminate()` to call `m_child.terminate()`
  - Make `PodmanProcess` constructible from a running `boost::process::child` and its associated pipe streams passed via constructor
  - _Requirements: 3_

- [ ] 3. Implement `PodmanSandbox::run()`
  - In `sandbox/podman/podman_sandbox.cpp`, replace the stub body with real logic that builds a `podman run` argument vector from `SandboxConfig`: always include `--rm`; add `--network=none` when `!config.network_enabled`; add `--read-only` when `config.read_only`; translate each `Mount` to `-v host:sandbox:ro` or `:rw`; translate each `env` entry to `-e KEY=VALUE`; add `--memory=N` and `--cpus=N` when the optional fields are set; append `config.image` then `config.command`
  - Spawn the child via `boost::process::child` with `boost::process::std_out` piped to an `ipstream`
  - Wrap any launch exception in a `std::runtime_error` with a descriptive message; never return `nullptr`
  - Construct and return `std::make_unique<PodmanProcess>(std::move(child), std::move(stdout_pipe))`
  - Add `#include <sandbox/podman/podman_process.hpp>` to `podman_sandbox.cpp`
  - _Requirements: 2, 3_

- [ ] 4. Fix `ImporterClient::fetch()`
  - In `compiler/importer_client.cpp`, replace the stub body of `fetch()` with a correct `SandboxConfig`: set `image = "latte-roaster"`, `network_enabled = false`, `read_only = true`, leave `command` empty so the Dockerfile `CMD` is used
  - Call `auto process = m_sandbox->run(config)` and store the result
  - Enter a `readLine()` loop; for each line parse it with `nlohmann::json` inside a try/catch; dispatch on `"type"`: `"started"` → `spdlog::info("[roaster] Starting...")`, `"progress"` → `spdlog::info("[roaster] Progress: {}%", value)`, `"completed"` → `spdlog::info("[roaster] Completed. Artifact: {}", artifact)`; on parse failure log a warning and continue
  - After the loop call `int code = process->wait()` and log an error if `code != 0`
  - _Requirements: 4_

- [ ] 5. Fix CMake linkage
  - In `compiler/CMakeLists.txt` add `latte_sandbox` to the `target_link_libraries` block for `latte_grind` so sandbox headers and symbols resolve at link time
  - In `sandbox/CMakeLists.txt` confirm that `podman/podman_process.cpp` is picked up by the glob or add it explicitly to the source list
  - _Requirements: 5_

- [ ] 6. Build and smoke-test end-to-end
  - Run `cmake --build build` and fix any compile or linker errors
  - Run `./build/latte grind qwen3-0.6b` and confirm the terminal prints the started message, four progress lines (25, 50, 75, 100), and the completed line with artifact `qwen.latte`
  - Run `podman ps -a` after the command exits and confirm no `latte-roaster` container remains
  - Confirm the process exits with code 0
  - _Requirements: 5, 6_
