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
)

target_link_libraries(${PROJECT_NAME}
    ${GLFW_LIBRARIES}
    ${OpenCV_LIBS}
    -lGL
)

message("opencv all: " ${OpenCV_LIBS})
foreach(cvmodule ${OpenCV_LIBS})
    message("opencv: " ${cvmodule})
endforeach(cvmodule)
