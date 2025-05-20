cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(GKS_PROJECT_NAME "GKS" CACHE STRING "Project name for IDEs")
project(${GKS_PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(misc_target)
include(set_cxx_standard)
include(target_folder)

include(CTest)

set_cxx_standard(17)

add_subdirectory(gks)
if(BUILD_TESTING)
    add_subdirectory(test)
endif()

misc_target(FILES
    "CMakePresets.json"
    "ReadMe.md"
)
misc_group_sources("CMake Scripts" FILES
    "cmake/misc_target.cmake"
    "cmake/set_cxx_standard.cmake"
    "cmake/target_folder.cmake")
misc_group_sources("Editing" FILES
    ".clang-format"
    ".editorconfig")
misc_group_sources("Git" FILES
    ".gitignore"
    ".gitmodules")
misc_group_sources("GitHub" FILES
    ".github/workflows/cmake.yml")
misc_group_sources("VcPkg" FILES
    "vcpkg.json")
