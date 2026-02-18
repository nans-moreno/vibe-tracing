cmake_minimum_required(VERSION 3.16)
project(vibe_tracing C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set GLFW3 and GLEW paths on Windows
if(WIN32)
    set(GLFW3_INCLUDE_DIR "C:/dependencies/glfw-3.3.10.bin.WIN64/include")
    set(GLFW3_LIB_DIR "C:/dependencies/glfw-3.3.10.bin.WIN64/lib-vc2022")
    set(GLEW_INCLUDE_DIR "C:/dependencies/glew-2.2.0/include")
    set(GLEW_LIB_DIR "C:/dependencies/glew-2.2.0/lib/Release/x64")
endif()

# Find packages
find_package(OpenGL REQUIRED)

# Add source files
set(SOURCES
    main_simple.c
    graphics.c
)

set(HEADERS
    raytracer.h
    ui.h
    graphics.h
    config.h
)

# Create executable
add_executable(vibe_tracing ${SOURCES})

# Link libraries
target_link_libraries(vibe_tracing PRIVATE
    OpenGL::GL
    ${GLFW3_LIB_DIR}/glfw3.lib
    ${GLEW_LIB_DIR}/glew32.lib
)

# Include directories
target_include_directories(vibe_tracing PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GLFW3_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
)

# Compiler flags
if(MSVC)
    target_compile_options(vibe_tracing PRIVATE /W4 /O2)
else()
    target_compile_options(vibe_tracing PRIVATE -Wall -Wextra -O2)
endif()
