# Quakaster
## A custom-built, UNIX-flavored, Quake-inspired game engine.

Quakaster is a minimal, modern game engine built around data-oriented design, composable systems, and a UNIX-inspired architecture.

## Core Features

### Cross-Platform Rendering (GL-minimally stable, VK-planned)
- OpenGL and Vulkan backends.
- PIMPL-based Rendering Hardware Interface (RHI).
- Kernel-queryable Graphics Abstraction Layer (GFX).

### Data-Oriented Entity-Component-System (in progress)
- Powered by EnTT.
- Minimal, engine-provided component and system library.
- Unified serialization interfaces for KeyValues and binary data.

### Engine Kernel (in progress)
A permission-aware intermediary between engine consumers and virtual drives.

Virtual drives are context-specific, in-memory resource databases that expose engine services through a unified interface.

**Core Virtual Drives**

| Drive | Responsibility |
|---|---|
| `proc:/` | Job queues and scheduling |
| `evt:/` | I/O event queues |
| `net:/` | Network packet queues |
| `prefab:/` | Entity prototypes and templates |
| `rsrc:/` | Resource management |
| `world:/` | Game state and entity management |

Additional kernel responsibilities:
- Memory allocation.
- Game mod permissions.
- Resource access and resolution.

### qsh — Quakaster Shell (planned)
- Bash-inspired command interface to the engine kernel.
- Context-aware command execution.
- Familiar syntax, new context.

### Entity Shading Language (ESL)
A C-like DSL for parallel entity processing, currently in development.


## Quick Start

Clone the repository and build Quakaster using CMake.

### Application Lifecycle

At a high level, Quakaster applications follow this execution model:

1. **Define an Application** — Implement the application interface and define its lifecycle.
2. **Initialize the Kernel** — Configure the engine's central service manager.
3. **Mount VDrives** — Register the virtual drives required by the application.
4. **Create Resources & Services** — Initialize the resources and services exposed through those drives.
5. **Register Jobs** — Add workloads to `proc:/` and configure their execution dependencies.
6. **Run** — Let the scheduler coordinate execution until the application terminates.

```text
Application
    |
    v
  Kernel
    |
    v
 Mount VDrives
    |
    v
Create Resources / Services
    |
    v
 Register Jobs
    |
    v
 Scheduler (proc:/)
    |
    v
 Application Runtime
```
