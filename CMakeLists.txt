cmake_minimum_required(VERSION 3.10)
project(MyHttpServer VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -pedantic -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fsanitize=address -pedantic -Wall")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -g -pedantic -Wall")
set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES
    src/*.c  # Collect all .c files in the src directory
)

include_directories(
  ${CMAKE_SOURCE_DIR}/lib/
)

add_executable(MyHttpServer
  ${SOURCES}
)
