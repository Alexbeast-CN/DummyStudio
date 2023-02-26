cmake_minimum_required(VERSION 3.0)
project(DummyStudio)

set(CMAKE_CXX_STANDARD 14)

# find OpenGL
find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_DIR thirdparty/glfw) # Set this to point to an up-to-date GLFW repo
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)

file(GLOB SRC
        thirdparty/imgui/*.h
        thirdparty/imgui/*.cpp
        )

include_directories(thirdparty/imgui)

add_executable(${PROJECT_NAME} ${SRC} main.cpp)
target_link_libraries(${PROJECT_NAME}
                        ${OPENGL_LIBRARIES}
                        glfw)




