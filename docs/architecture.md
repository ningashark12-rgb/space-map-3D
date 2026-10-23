# Architecture

## Overview

The project is a **3D Space Map** with A* pathfinding, built in C++ using OpenGL for rendering. It is split into 4 modules:

| Module | Responsibility | Owner |
|--------|---------------|-------|
| `core/` | Data model — stars, graph structure | A |
| `pathfinding/` | Pathfinding — A*, Dijkstra, BFS | B |
| `render/` | OpenGL rendering, camera | C |
| `ui/` | User interface, selection | C |

---

## Module Dependencies

```
main.cpp
├── core/    (A provides)
├── pathfinding/      (B provides, depends on core)
├── render/  (C provides, depends on core)
└── ui/      (C provides, depends on core + ai)
```

**A is the foundation. B and C both depend on A.**

---

## OOP Design

### Class Hierarchy

```
CelestialObject (abstract)
├── Star
└── Nebula

PathFinder (abstract)
├── AStarPathFinder
├── DijkstraPathFinder
└── BFSPathFinder

Heuristic (abstract)
├── EuclideanHeuristic
└── ManhattanHeuristic
```

### OOP Concepts Used

| Concept | Where | Example |
|---------|-------|---------|
| **Abstraction** | Abstract base classes | `CelestialObject`, `PathFinder`, `Heuristic` |
| **Inheritance** | Subclasses | `Star : public CelestialObject` |
| **Polymorphism** | Virtual methods | `findPath()`, `render()`, `estimate()` |
| **Encapsulation** | Private members, public API | `Star` hides internal data |
| **Composition** | Has-a relationships | `StarMap` contains `std::vector<Star*>` |
| **Design Patterns** | Strategy, Factory | PathFinders are interchangeable |

### Design Patterns

- **Strategy Pattern** — `PathFinder` subclasses are interchangeable (A*, Dijkstra, BFS)
- **Factory Pattern** — `StarData` creates stars from CSV or randomly
- **Observer Pattern** — UI updates when selection changes

---

## STL Usage

| STL Feature | Where | Purpose |
|-------------|-------|---------|
| `std::vector` | `StarMap`, `PathFinder` | Store stars and paths |
| `std::unordered_map` | `StarMap` | Adjacency list, O(1) lookup |
| `std::priority_queue` | `AStarPathFinder` | Open set (min-heap by f-score) |
| `std::set` | `AStarPathFinder` | Closed set (fast membership) |
| `std::optional` | `StarMap` | "Not found" queries |
| `std::unique_ptr` | `StarMap` | Ownership of stars |
| `std::algorithm` | Throughout | `std::sort`, `std::find_if`, `std::transform` |

---

## Templates

| Template | Purpose |
|----------|---------|
| `Graph<T>` | Generic graph for any node type |
| `PathFinder<T>` | Generic pathfinding for any graph |
| `Heuristic<T>` | Generic heuristic for any node type |
| `PathResult<T>` | Success/failure wrapper |

### Example

```cpp
template <typename T>
struct PathResult {
    bool found;
    T path;
    float cost;
    std::string error;
};
```

---

## Data Flow

```
1. StarData loads stars from CSV (or generates randomly)
2. StarMap stores stars and builds adjacency list
3. PathFinder uses StarMap to find paths
4. Renderer displays stars and paths
5. UI handles user interaction
```

---

## Integration Points

| When | What | From → To |
|------|------|-----------|
| Week 1 | Star class ready | A → B, C |
| Week 2 | StarMap ready | A → B |
| Week 3 | A* findPath ready | B → C |
| Week 4 | Camera ready | C → A, B |
| Week 5 | Full integration | All |

**Integrate weekly. Don't wait until the end.**

---

## File Structure

```
src/
├── core/       (A)
├── pathfinding/         (B)
├── render/     (C)
├── ui/         (C)
└── main.cpp

tests/          (All)
assets/         (C)
docs/           (All)
```

---

## Build System

- **Visual Studio** — IDE with built-in build system (no CMake)
- **SFML** — window and input
- **OpenGL** — 3D rendering
- **GLM** — 3D math
- **Googel Test** — unit testing

---

## Testing Strategy

| Module | Tests |
|--------|-------|
| `core/` | Star creation, StarMap graph, data loading |
| `pathfinding/` | PathFinder correctness, heuristic admissibility |
| `render/` | Camera matrices, shader compilation |
| `ui/` | Selection logic |

---

## See Also

- [`docs/interfaces.md`](interfaces.md) — Function signatures
- [`docs/report.md`](report.md) — Final report
- [`README.md`](../README.md) — Project overview
