# Final Report — 3D Space Map with A* Pathfinding

**Course:** PPOIS  
**Team:**
- Member A — Core & Graph
- Member B — Search algorithms & 3D Rendering

**Date:** [fill in when submitting]  
**Repository:** https://github.com/ningashark12-rgb/space-map-3D

---

## 1. Project Idea

The project is a **3D Space Map** — an interactive visualization of a star field where users can explore stars in 3D space, select any two stars, and find the optimal path between them using **A* pathfinding**.

The goal is to demonstrate:
- **OOP** — abstract base classes, inheritance, polymorphism, strategy pattern
- **STL** — containers, algorithms, smart pointers
- **C++ Templates** — generic graph, pathfinder, and result types
- **3D Rendering** — OpenGL with SFML
- **Pathfinding algorithm** — A* algorithm with admissible heuristics

---

## 2. Requirements

### Functional Requirements
- Load and display a 3D star field
- Navigate with camera controls (WASD + mouse)
- Click to select stars
- Find shortest path between two stars using A*
- Visualize the path
- Display star information in a UI panel

### Non-Functional Requirements
- Smooth 60 FPS rendering
- Real-time pathfinding on 1000+ stars
- Cross-platform (Linux, Windows, macOS)
- Modular architecture
- Unit tested

---

## 3. Architecture

### Modules

| Module | Responsibility | Owner |
|--------|---------------|-------|
| `core/` | Data model — stars, graph structure | A |
| `pathfinding/` | Pathfinding — A*, Dijkstra, BFS | B |
| `render/` | OpenGL rendering, camera | B |
| `ui/` | User interface, selection | B |

See [`docs/architecture.md`](architecture.md) for full details.

### Class Hierarchy

```
CelestialObject (abstract)
├── Star
└── Nebula

Pathfinder (abstract)
├── AStarPathfinder
├── DijkstraPathfinder
└── BFSPathfinder

Heuristic (abstract)
├── EuclideanHeuristic
└── ManhattanHeuristic
```

---

## 4. Implementation

### Core Module (A)
- `CelestialObject` — abstract base for all space objects
- `Star` — concrete star with position, mass, type
- `StarMap` — graph structure holding stars and their connections
- `StarData` — loads stars from CSV or generates randomly

### AI Module (B)
- `Pathfinder` — abstract base class
- `AStarPathfinder` — A* with configurable heuristic
- `DijkstraPathfinder` — Dijkstra's algorithm
- `BFSPathfinder` — Breadth-first search
- `Heuristic` — abstract + Euclidean implementation
- `PathResult<T>` — templated result type
- `Renderer` — draws stars and paths
- `Camera` — 3D camera with WASD + mouse controls
- `Shader` — OpenGL shader wrapper
- `StarRenderer` — renders individual stars
- `SphereMesh` — 3D sphere geometry
- `UI` — info panel, filters
- `Selection` — click-to-select stars

---

## 5. Technologies Used

| Technology | Purpose |
|------------|---------|
| **C++17** | Language |
| **Visual Studio** | IDE with built-in build system |
| **SFML 2.5** | Window, input |
| **OpenGL 3.3** | 3D rendering |
| **GLM** | 3D math |
| **Google Test** | Unit testing |
| **GitHub** | Version control, PRs, task board |

### OOP Concepts
- Abstract base classes, inheritance, polymorphism
- Encapsulation, composition
- Strategy pattern (pathfinders)
- Factory pattern (star creation)
- Observer pattern (UI updates)

### STL Usage
- `std::vector`, `std::unordered_map`, `std::priority_queue`
- `std::optional`, `std::unique_ptr`
- `std::sort`, `std::find_if`, `std::transform`

### Templates
- `Graph<T>`, `Pathfinder<T>`, `Heuristic<T>`, `PathResult<T>`

---

## 6. Testing

### Unit Tests

| Module | Tests |
|--------|-------|
| `core/` | Star creation, StarMap graph, data loading |
| `pathfinding/` | Pathfinder correctness, heuristic admissibility |
| `render/` | Camera matrices, shader compilation |
| `ui/` | Selection logic |

### Test Framework
- **Google Test** — unit testing framework
- Run tests from Visual Studio Test Explorer, or via command line

### Example Tests
```cpp
#include <gtest/gtest.h>
TEST(AStarTest, FindsShortestPath) { ... }
TEST(AStarTest, ReturnsEmptyWhenNoPath) { ... }
TEST(HeuristicTest, EuclideanIsAdmissible) { ... }
```

---

## 7. Development Process

### GitHub Workflow
- **Branches:** one per feature (`feature/astar`, `feature/camera`, etc.)
- **Pull Requests:** each feature merged via PR with review
- **Task Board:** GitHub Projects with To Do / In Progress / Done columns
- **Commits:** small, descriptive, ~50+ commits total

### Timeline (6 weeks)
| Week | Focus |
|------|-------|
| 1 | Setup, Star class, Pathfinder base, Visual Studio project |
| 2 | StarMap, BFS, Dijkstra, shaders |
| 3 | Data loading, A*, star rendering, camera |
| 4 | Integration, path rendering, UI panel |
| 5 | Testing, bug fixes, docs |
| 6 | Polish, README, final report, submission |

### Integration Points
| When | What | From → To |
|------|------|-----------|
| Week 1 | Star class ready | A → B |
| Week 2 | StarMap ready | A → B |
| Week 3 | A* findPath ready | B (Internal) |
| Week 4 | Camera ready | B(Internal) |
| Week 5 | Full integration | Both |

---

## 8. Individual Contributions

### Member A — Core & Graph
- Implemented `CelestialObject`, `Star`, `StarMap`
- Implemented data loading from CSV
- Wrote unit tests for core module
- [add specific commits/PRs]

### Member B — Search & Rendering 
- Implemented `Pathfinder` base class
- Implemented A*, Dijkstra, BFS
- Implemented heuristics
- Wrote unit tests for AI module
- Implemented OpenGL window, shaders
- Implemented `Camera`, `StarRenderer`
- Implemented `UI`, `Selection`
- Wrote unit tests for render/ui modules
- [add specific commits/PRs]

---

## 9. Final Results

- Working 3D space map with 1000+ stars
- A* pathfinding between any two stars
- Interactive camera and selection
- Visualization of paths
- Unit tests passing
- Complete documentation

### Screenshots
[add screenshots here]

### Demo Video
[add link if applicable]

---

## 10. Lessons Learned

- **OOP depth** — abstract base classes and design patterns make the code extensible
- **STL usage** — containers and algorithms simplify complex logic
- **Templates** — generic graph and pathfinder allow reuse
- **Teamwork** — GitHub branches and PRs enable parallel work
- **Challenges** — [add what was hard]

---

## 11. References

- A* Pathfinding: https://en.wikipedia.org/wiki/A*_search_algorithm
- SFML Documentation: https://www.sfml-dev.org/documentation/
- OpenGL Tutorials: https://learnopengl.com/
- Google Test: https://github.com/google/googletest

---
---

## Appendix: Development Log

This appendix documents the actual development timeline and individual contributions.

### Member A — Core & Graph

### Member B — Search algorithms & Pathfinding & Rendering
- Oct 8: Created GitHub repository with README, LICENSE, .gitignore
- Oct 8: Authored `docs/interfaces.md` (function signature agreements)
- Oct 8: Authored `docs/architecture.md` (module design, OOP/STL/templates plan)
- Oct 8: Authored `docs/report.md` (final report skeleton)
- Oct 8: Set up folder structure (src/core, src/pathfinding, src/render, src/ui, tests, assets)
- Oct 8: Created GitHub Projects board with To Do / In Progress / Done columns
- Oct 8: Created and assigned 22 issues to team members
- Oct 10: Renamed `src/ai/` → `src/pathfinding/` (pathfinding is not AI)
- Oct 10: Updated docs to reflect module rename
- Oct 10: Switched build system from CMake to Visual Studio
- Oct 10: Switched testing framework from Catch2 to Google Test
- Oct 19: Implemented all pathfinding headers (`Pathfinder.h`, `Heuristic.h`, `PathResult.h`, `AStarPathfinder.h`)
- Oct 19: Implemented algorithms (`BFSPathfinder.cpp`, `DijkstraPathfinder.cpp`, `AStarPathfinder.cpp`)
- Oct 19: Set up Google Test framework and wrote initial unit tests
- Oct 19: Updated `interfaces.md` to pass `StarMap*` to `findPath()`
- Oct 19: **BLOCKED**: Cannot compile or run full integration tests because `src/core/Star.h` and `StarMap.h` (Member A) are not yet in the repository. 
- Oct 19: Note: Current unit tests are edge-case skeletons. Full graph-traversal tests will be added as soon as Member A pushes the Core module.

- Oct 23: Group restructured. Member C and original Member A removed from the repository due to lack of contribution. Member B took over the Rendering module. A new Member A joined to take over the Core module.

