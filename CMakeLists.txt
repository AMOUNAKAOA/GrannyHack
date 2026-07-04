cmake_minimum_required(VERSION 3.18)

project(GrannyHack)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Android NDK settings (if using Android NDK)
if(ANDROID)
    message(STATUS "Building for Android API level ${ANDROID_PLATFORM}")
endif()

# Find required packages
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    # Add ImGui include path (optional)
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui
    # Add any other third-party includes
)

# Source files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# Try to locate ImGui directory in common locations (case-sensitive filesystem aware)
set(POSSIBLE_IMGUI_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImGui"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui"
)
set(IMGUI_DIR "")
foreach(dir IN LISTS POSSIBLE_IMGUI_DIRS)
    if(EXISTS "${dir}/imgui.cpp")
        set(IMGUI_DIR "${dir}")
        break()
    endif()
endforeach()

# ImGui sources (only add them if they exist to avoid CMake errors when imgui is not vendored)
if(IMGUI_DIR)
    message(STATUS "Found ImGui in ${IMGUI_DIR}; including ImGui sources in the build")
    set(IMGUI_SOURCES
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui_demo.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_widgets.cpp"
        "${IMGUI_DIR}/imgui_tables.cpp"
        "${IMGUI_DIR}/imgui_impl_opengl3.cpp"
        "${IMGUI_DIR}/imgui_impl_android.cpp"
    )
else()
    message(WARNING "ImGui sources not found in standard locations (imgui/, ImGui/, src/imgui/, src/ImGui/). Building without ImGui. If you need ImGui, add it as a submodule or copy the sources into one of those directories.")
    set(IMGUI_SOURCES "")
endif()

# Create library or executable
if(ANDROID)
    # Build as shared library for Android
    add_library(${PROJECT_NAME} SHARED
        ${SOURCES}
        ${IMGUI_SOURCES}
    )
else()
    # Build as executable for desktop
    add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${IMGUI_SOURCES}
    )
endif()

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        OpenGL::GL
)

# Android-specific linking
if(ANDROID)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            android
            EGL
            GLESv3
    )
endif()

# Compiler flags for optimization
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()
