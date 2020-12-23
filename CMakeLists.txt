cmake_minimum_required(VERSION 3.0)
project(navec_cpp)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS "/EHsc")
set(CMAKE_CXX_FLAGS "/EHsc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
SET(CMAKE_CXX_FLAGS_DEBUG  "-MDd -Zi -Ob0 -Od -RTC1  -_Debug") 
add_definitions(-DUNICODE -D_UNICODE)
add_subdirectory(zlib)

set(HEADERS "${CMAKE_SOURCE_DIR}/navec.h"
)
set(SOURCES "${CMAKE_SOURCE_DIR}/main.cpp"
            "${CMAKE_SOURCE_DIR}/navec.cpp"
			"${CMAKE_SOURCE_DIR}/numpyLib/cnpy.cpp"
)
add_definitions(-DUNICODE -D_UNICODE)

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/zlib")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/numpyLib")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} )
target_link_libraries(${PROJECT_NAME} zlib)
set_target_properties( ${PROJECT_NAME} PROPERTIES  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

file(COPY ${PROJECT_SOURCE_DIR}/data DESTINATION ${CMAKE_SOURCE_DIR}/build/Debug)
file(COPY ${PROJECT_SOURCE_DIR}/data DESTINATION ${CMAKE_SOURCE_DIR}/build/Release)