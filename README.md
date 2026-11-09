# Quakaster
## A custom-built, UNIX-flavored, Quake-inspired game engine.

Quakaster is a minimal, modern game engine built around data-oriented design, composable systems, and a UNIX-inspired architecture.

## Core Features

### Cross-Platform Rendering
- OpenGL and Vulkan backends.
- PIMPL-based Rendering Hardware Interface (RHI).
- Kernel-queryable Graphics Abstraction Layer (GFX).

### Data-Oriented Entity-Component-System
- Powered by EnTT.
- Minimal, engine-provided component and system library.
- Unified serialization interfaces for KeyValues and binary data.

### Engine Kernel
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

### qsh — Quakaster Shell
- Bash-inspired command interface to the engine kernel.
- Context-aware command execution.
- Familiar syntax, new context.

### Entity Shading Language (ESL)
A C-like DSL for parallel entity processing, currently in development.
