find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable( MRTs MRTs.c )
target_link_libraries( MRTs Common ${OpenCV_LIBS})
