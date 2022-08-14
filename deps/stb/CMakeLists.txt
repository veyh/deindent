cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(stb C ASM)
add_library(stb STATIC "${CMAKE_CURRENT_LIST_DIR}/stb.c")
