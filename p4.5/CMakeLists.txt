cmake_minimum_required(VERSION 3.0.0)
project(p4.5 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(p4.5 main.cpp Utils.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(
    p4.5
    GL
    GLU
    GLEW
    glfw
)
