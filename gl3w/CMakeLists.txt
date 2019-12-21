cmake_minimum_required(VERSION 3.1)

project(gl3w)

add_library(gl3w STATIC "gl3w.c")
target_include_directories(gl3w INTERFACE .)
