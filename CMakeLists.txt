project (CornerDetection)

cmake_minimum_required(VERSION 2.8)

find_package(OpenCV REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR/build})

include_directories (${OpenCV_INCLUDE_DIRS})

if (EXISTS "${PROJECT_SOURCE_DIR}/CornerDetector.cpp")
	add_executable (CornerDetector CornerDetector.cpp)
	target_link_libraries (CornerDetector ${OpenCV_LIBS})
endif ()
