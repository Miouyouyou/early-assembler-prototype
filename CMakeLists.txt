cmake_minimum_required(VERSION 2.8)

# User defined
project(OpenGLInterfaces)

set (CommonSources armv7-arm.c)

include_directories(.)

add_executable(LibraryTest main.c ${CommonSources})
add_executable(ElfTest elf.c ${CommonSources})
add_executable(DataStructuresTest test-data-structures.c ${CommonSources})

