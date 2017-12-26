cmake_minimum_required (VERSION 3.5)
project (gles_video_viewer)

set (CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

find_package(OpenCV REQUIRED highgui core imgproc)

include_directories(
    ${GLFW_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
    main.cpp
    gl_handler.cpp
    gl_handler.h
    shader.frag # only added here to add file to project in IDE
    shader.vert # only added here to add file to project in IDE
    passthrough.frag # only added here to add file to project in IDE
    passthrough.vert # only added here to add file to project in IDE
)

target_link_libraries(${PROJECT_NAME}
    ${GLFW_LIBRARIES}
    ${OpenCV_LIBS}
    -lGL
)
