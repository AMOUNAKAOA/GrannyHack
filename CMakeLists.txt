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
    # Add ImGui include path
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    # Add any other third-party includes
)

# Source files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# ImGui sources (adjust path as needed)
set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_impl_android.cpp
)

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