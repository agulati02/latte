# LattE Project Overview

## Vision

LattE is a cross-platform C++ inference runtime and CLI for open-weight LLMs.

### Goals
- Native C++ inference runtime — no Python at runtime
- Portable `.latte` intermediate representation for compiled models
- Python is used only during model compilation (the `grind` phase)
- Cross-platform CLI (`latte grind`, `latte serve`, ...)
- Backend-agnostic sandboxed model compilation

---

## End-to-End Workflow

```
latte grind <huggingface-model>
        │
        ▼
latte-roaster (Python, runs inside sandbox)
        │
        ▼
Transformers + trust_remote_code
        │
        ▼
torch.export
        │
        ▼
LattE IR
        │
        ▼
model.latte
        │
        ▼
latte serve model.latte
```

---

## Tech Stack

| Layer | Technology |
|---|---|
| Language | C++20 |
| Build | CMake + vcpkg |
| CLI parsing | CLI11 |
| Logging | spdlog |
| HTTP | cpr |
| Serialization | nlohmann/json |
| Process execution | Boost.Process |
| Sandbox backend | Podman (initial); Docker/Bubblewrap planned |
| Model compilation | Python, Transformers, torch.export |

---

## Hard Rules

- **The C++ runtime never executes Python directly.** Python only runs inside the sandbox via the roaster.
- **The importer protocol is independent from the transport.** JSON today; can change without touching the sandbox.
- **Favor dependency inversion and modular libraries.** Each CMake target has a single, clearly bounded responsibility.
