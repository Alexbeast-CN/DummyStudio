cmake_minimum_required(VERSION 3.0)
project(DummyStudio)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

# GLFW
set(HELLOIMGUI_USE_GLFW_OPENGL3 ON)

set(GLFW_DIR thirdparty/glfw) # Set this to point to an up-to-date GLFW repo
message(STATUS "Library glfw")
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} glfw EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)

# imgui
set(IMGUI_DIR thirdparty/imgui)
message(STATUS "Library imgui")
file(GLOB imgui_sources ${IMGUI_DIR}/*.h ${IMGUI_DIR}/*.cpp)
set(imgui_sources ${imgui_sources} ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp ${IMGUI_DIR}/misc/cpp/imgui_stdlib.h)
add_library(imgui ${imgui_sources})
target_link_libraries(imgui PUBLIC ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR})

# OpenGL loader
option(HELLOIMGUI_USE_GLAD "Use Glad OpenGl loader" ON)
find_package(OpenGL REQUIRED)
set(HELLOIMGUI_SOLUTIONFOLDER ${CMAKE_CURRENT_LIST_DIR}/thirdparty/hello_imgui)
include(thirdparty/OpenGL_Loaders/glad.cmake)

# hello_imgui
add_subdirectory(${HELLOIMGUI_SOLUTIONFOLDER} hello_imgui EXCLUDE_FROM_ALL)

# Ads
include_directories("C:/TwinCAT/AdsApi/TcAdsDll/Include")
link_directories(${PROJECT_NAME} PRIVATE "C:/TwinCAT/AdsApi/TcAdsDll/Lib")

#jaka
set(JAKA_DIR ${CMAKE_CURRENT_LIST_DIR}/thirdparty/jaka)
include_directories(${JAKA_DIR}/include)

if (WIN32)
    LINK_DIRECTORIES(${JAKA_DIR}/lib_windows)
    file(COPY ${JAKA_DIR}/lib_windows/jakaAPI.dll DESTINATION ${CMAKE_BINARY_DIR})
elseif(UNIX)
    LINK_DIRECTORIES(${JAKA_DIR}/lib_linux)
    file(COPY ${JAKA_DIR}/lib_linux/jakaAPI.so DESTINATION ${CMAKE_BINARY_DIR})
endif ()
LINK_LIBRARIES(jakaAPI)

# executable
file (GLOB SRC ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME}
                        Threads::Threads
                        hello_imgui
                        TcAdsDll)

# Assets
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})


