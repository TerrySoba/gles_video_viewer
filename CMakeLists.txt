cmake_minimum_required (VERSION 3.5)
project (gles_video_viewer)

set (CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${GLFW_LIBRARIES}
    -lGL
)
