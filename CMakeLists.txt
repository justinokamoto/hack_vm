cmake_minimum_required(VERSION 3.5)

project(vm_translator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TRANSLATOR_TARGET ${PROJECT_NAME})

add_executable(${TRANSLATOR_TARGET}
  src/parser
  src/writer.cpp
  src/main.cpp)

add_custom_target(test
  COMMAND run-tests
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
