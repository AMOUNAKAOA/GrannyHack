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
    # Add any other third-party includes
)

# Source files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# ImGui sources (only add them if they exist to avoid CMake errors when imgui is not vendored)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp")
    message(STATUS "Found ImGui sources; including them in the build")
    set(IMGUI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_impl_opengl3.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_impl_android.cpp"
    )
else()
    message(WARNING "ImGui sources not found in ${CMAKE_CURRENT_SOURCE_DIR}/imgui — building without ImGui. If you need ImGui, add it as a submodule or copy the sources into the imgui/ directory.")
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
