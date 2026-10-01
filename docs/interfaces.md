# Interface Agreements

**Project:** 3D Space Map with A* Pathfinding  
**Team:**
- Member A — Core & Graph
- Member B — AI & Pathfinding
- Member C — 3D Rendering & UI

---

## Purpose

This document defines the **interfaces** (function signatures) that each member promises to provide to the team. We agree on these **before coding** so we can work in parallel without breaking each other's code.

## Rules

1. **Agree on Day 1** — before anyone writes code
2. **Don't change without team agreement** — if you must change, message everyone
3. **If you change, everyone updates** — no exceptions
4. **Test against these interfaces** — your module must work with the agreed API
5. **Keep it minimal** — only expose what others need

---

## Member A Provides (Core & Graph)

```cpp
// ============================================
// core/Star.h
// ============================================
#pragma once
#include <string>
#include <glm/glm.hpp>

enum class StarType {
    MainSequence,
    RedGiant,
    WhiteDwarf,
    NeutronStar
};

class Star {
public:
    Star(std::string name, glm::vec3 position, float mass, StarType type);
    glm::vec3 getPosition() const;
    std::string getName() const;
    float getMass() const;
    StarType getType() const;

private:
    std::string name_;
    glm::vec3 position_;
    float mass_;
    StarType type_;
};

// ============================================
// core/StarMap.h
// ============================================
#pragma once
#include <vector>
#include <memory>
#include <optional>
#include <unordered_map>
#include "Star.h"

class StarMap {
public:
    // Used by B (pathfinding)
    std::vector<Star*> getNeighbors(Star* star) const;
    float getDistance(Star* a, Star* b) const;
    std::vector<Star*> getAllStars() const;

    // Used by C (rendering)
    std::optional<Star*> findByName(const std::string& name) const;
    Star* findNearest(const glm::vec3& position) const;

    // Setup
    void addStar(std::unique_ptr<Star> star);
    void addRoute(Star* a, Star* b);

private:
    std::vector<std::unique_ptr<Star>> stars_;
    std::unordered_map<Star*, std::vector<Star*>> adjacency_;
};
```

---

## Member B Provides (AI & Pathfinding)

```cpp
// ============================================
// ai/Pathfinder.h
// ============================================
#pragma once
#include <vector>
#include <string>
#include "core/Star.h"

class Pathfinder {
public:
    virtual ~Pathfinder() = default;
    virtual std::vector<Star*> findPath(Star* start, Star* goal) = 0;
    virtual std::string name() const = 0;
};

// ============================================
// ai/PathResult.h
// ============================================
#pragma once
#include <vector>
#include <string>
#include "core/Star.h"

template <typename T>
struct PathResult {
    bool found;
    T path;
    float cost;
    std::string error;

    static PathResult success(T p, float c) {
        return { true, p, c, "" };
    }

    static PathResult failure(std::string err) {
        return { false, T{}, 0.0f, err };
    }
};

// ============================================
// ai/AStarPathfinder.h
// ============================================
#pragma once
#include "Pathfinder.h"
#include "Heuristic.h"
#include <memory>

class AStarPathfinder : public Pathfinder {
public:
    AStarPathfinder(std::unique_ptr<Heuristic> heuristic);
    std::vector<Star*> findPath(Star* start, Star* goal) override;
    std::string name() const override { return "A*"; }

private:
    std::unique_ptr<Heuristic> heuristic_;
};

// ============================================
// ai/Heuristic.h
// ============================================
#pragma once
#include "core/Star.h"

class Heuristic {
public:
    virtual ~Heuristic() = default;
    virtual float estimate(Star* a, Star* b) const = 0;
};

class EuclideanHeuristic : public Heuristic {
public:
    float estimate(Star* a, Star* b) const override;
};
```

---

## Member C Provides (3D Rendering & UI)

```cpp
// ============================================
// render/Camera.h
// ============================================
#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const;
    void update(float dt);
};

// ============================================
// ui/Selection.h
// ============================================
#pragma once
#include <vector>
#include "core/Star.h"
#include "render/Camera.h"

class Selection {
public:
    Star* getStarAtScreenPos(int x, int y,
                             const std::vector<Star*>& stars,
                             const Camera& camera);
};
```

---

## Module Dependencies

```
main.cpp
├── core/    (A provides)
├── ai/      (B provides, depends on core)
├── render/  (C provides, depends on core)
└── ui/      (C provides, depends on core + ai)
```

**A is the foundation. B and C both depend on A.**

---

## Integration Points

| When | What | From → To |
|------|------|-----------|
| Week 1 Day 7 | Star class ready | A → B, C |
| Week 2 Day 7 | StarMap ready | A → B |
| Week 3 Day 1 | A* findPath ready | B → C |
| Week 3 Day 7 | Camera ready | C → A, B |
| Week 4 Day 1 | Full integration | All |

**Integrate weekly. Don't wait until the end.**

---

## Rules for Changes

If you need to change an interface:

1. Message the team first
2. Explain why the change is needed
3. Get agreement from affected members
4. Update this file in a PR
5. Update your code to match

**No silent changes. No surprises.**

---

## Sign-Off

By working on this project, we agree to these interfaces:

- **Member A:** _________________ Date: _______
- **Member B:** _________________ Date: _______
- **Member C:** _________________ Date: _______
