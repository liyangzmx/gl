cmake_minimum_required(VERSION 3.0.0)
project(p4.6.3 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(p4.6.3 main.cpp Utils.cpp Sphere.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_library(soil2 SHARED IMPORTED)
set_target_properties(
    soil2
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_CURRENT_LIST_DIR}/../SOIL2/lib/linux/libsoil2.a
)
include_directories(../SOIL2/src/SOIL2/)

target_link_libraries(
    p4.6.3
    GL
    GLU
    GLEW
    glfw
    soil2
)
