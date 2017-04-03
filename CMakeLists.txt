cmake_minimum_required(VERSION 2.8)

# User defined
project(OpenGLInterfaces)

set (Sources main.c)

include_directories(.)

add_executable(LibraryTest ${Sources})
add_executable(ElfTest elf.c)
