cmake_minimum_required(VERSION 2.8)

# User defined
project(OpenGLInterfaces)

set (Sources main.c armv7-arm.c)

include_directories(.)

add_executable(LibraryTest ${Sources})
add_executable(ElfTest elf.c armv7-arm.c)
