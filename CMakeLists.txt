cmake_minimum_required(VERSION 3.10)
project(ProcessCommunication C)

set(CMAKE_C_STANDARD 11)


add_executable(main src/main.c)


add_executable(ChildProcess src/ChildProcess.c)


if(WIN32)
    target_compile_definitions(main PRIVATE WIN32_LEAN_AND_MEAN)
    target_compile_definitions(ChildProcess PRIVATE WIN32_LEAN_AND_MEAN)
endif()


set_target_properties(main ChildProcess
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)