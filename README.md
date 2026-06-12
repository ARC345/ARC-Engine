# ARC-Engine

A 2D game engine built with C++20, OpenGL, and ECS architecture.

## Features

- **2D Renderer** — Batch-rendered quads, circles, lines with sprite/texture support
- **ECS Architecture** — Entity-component-system via EnTT, with a component registry system
- **Scene System** — Entity hierarchy, cameras, transform hierarchy
- **Physics** — Box2D integration for 2D physics (rigid bodies, collision)
- **Particle System** — 2D particle effects with modifiers
- **Scripting** — Native C++ entity controllers
- **Editor** — ImGui-based level editor with scene hierarchy, content browser, and property panels
- **Serialization** — YAML scene files (human-readable) and binary scene files (fast load/save)

## Architecture

```
ARC-Engine/           — Core engine library (static lib)
├── src/
│   ├── ARC/
│   │   ├── Core/         — Application, window, logging, platform detection
│   │   ├── Events/       — Event system (keyboard, mouse, window)
│   │   ├── GUI/          — ImGui layer + helpers
│   │   ├── Helpers/      — Math, conversions, utilities
│   │   ├── Input/        — Cross-platform input handling
│   │   ├── Objects/      — Primitive drawable objects
│   │   ├── Profiling/    — Instrumentation/timing
│   │   ├── Renderer/     — 2D renderer, shaders, textures, cameras
│   │   ├── Scene/        — Scene graph, ECS components, physics
│   │   └── Types/        — Vectors, colors, transforms, delegates
│   └── Platform/
│       ├── Linux/        — Linux window management, input, file dialogs
│       ├── OpenGl/       — OpenGL backend (buffers, shaders, textures)
│       └── Windows/      — Windows window management, input, file dialogs
ARC-Editor/           — Editor application (console app)
Sandbox/              — Example sandbox applications
Vendor/               — Third-party dependencies (via git submodules)
```

## Dependencies

| Library | Purpose |
|---------|---------|
| GLFW | Windowing and input |
| GLAD | OpenGL loader |
| GLM | Math library (SIMD-optimized vectors, matrices) |
| ImGui | Immediate-mode GUI (docking branch) |
| ImGuizmo | 3D gizmo controls |
| EnTT | ECS framework |
| Box2D | 2D physics engine |
| spdlog | Logging |
| yaml-cpp | YAML serialization |
| stb_image | Image loading |
| premake5 | Build system |

## Building

### Linux

```bash
# Install dependencies
sudo apt install build-essential libgl1-mesa-dev libglu1-mesa-dev \
  libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxext-dev \
  libwayland-dev libxkbcommon-dev libdbus-1-dev

# Or use the setup script
chmod +x Scripts/Linux-Setup.sh
./Scripts/Linux-Setup.sh

# Build
make config=release -j$(nproc)
```

### Windows (Visual Studio)

```powershell
# Download premake5 and generate VS solution
premake5 vs2022

# Build
msbuild ARC-Engine.sln /p:Configuration=Release /p:Platform=x64
```

## Using in Your Project

```cpp
#include <ARC.h>

class CMyGame : public ARC::CApplication
{
public:
    CMyGame() : ARC::CApplication("MyGame") {}
};

ARC::CApplication* ARC::CreateApplication()
{
    return new CMyGame();
}
```

## Math Library

ARC-Engine uses GLM internally for vector/matrix math but exposes its own type system (`FVec2`, `FVec3`, `FMat4`, etc.) so the backend can be swapped without changing game code. All float vector types delegate directly to GLM for SIMD-optimized operations.

| ARC Type | GLM Backend | Purpose |
|----------|-------------|---------|
| `FVec2` | `glm::vec2` | 2D float vector |
| `FVec3` | `glm::vec3` | 3D float vector |
| `FVec4` | `glm::vec4` | 4D float vector / color |
| `FTransform2D` | — | Position, rotation, scale |
| `SMath` | GLM wrappers | Dot, cross, distance, lerp, etc. |

## License

MIT
