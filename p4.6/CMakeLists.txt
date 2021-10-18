cmake_minimum_required(VERSION 3.0.0)
project(p4.6 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(p4.6 main.cpp Utils.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(
    p4.6
    GL
    GLU
    GLEW
    glfw
)
