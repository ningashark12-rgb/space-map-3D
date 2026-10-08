cmake_minimum_required(VERSION 3.15)
project(space_map_3d LANGUAGES CXX)

# ============================================
# C++ Standard
# ============================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================
# Build type
# ============================================
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ============================================
# Dependencies
# ============================================
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

# Catch2 for testing
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.10
)
FetchContent_MakeAvailable(Catch2)

# ============================================
# Main executable
# ============================================
set(SOURCES
    src/main.cpp
    # Core (A)
    src/core/CelestialObject.cpp
    src/core/Star.cpp
    src/core/StarMap.cpp
    src/core/StarData.cpp
    # AI (B)
    src/ai/AStarPathfinder.cpp
    src/ai/DijkstraPathfinder.cpp
    src/ai/BFSPathfinder.cpp
    src/ai/Heuristic.cpp
    # Render (C)
    src/render/Renderer.cpp
    src/render/Camera.cpp
    src/render/Shader.cpp
    src/render/StarRenderer.cpp
    # UI (C)
    src/ui/UI.cpp
    src/ui/Selection.cpp
)

add_executable(space_map ${SOURCES})

target_include_directories(space_map PRIVATE src)

target_link_libraries(space_map PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    OpenGL::GL
    glm::glm
)

# ============================================
# Tests
# ============================================
enable_testing()

set(TEST_SOURCES
    tests/test_main.cpp
    tests/test_star.cpp
    tests/test_starmap.cpp
    tests/test_pathfinder.cpp
    tests/test_heuristic.cpp
    tests/test_camera.cpp
)

add_executable(space_map_tests ${TEST_SOURCES})

target_include_directories(space_map_tests PRIVATE src)

target_link_libraries(space_map_tests PRIVATE
    Catch2::Catch2
    sfml-graphics
    sfml-window
    sfml-system
    OpenGL::GL
    glm::glm
)

add_test(NAME unit_tests COMMAND space_map_tests)
