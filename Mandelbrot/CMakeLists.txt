cmake_minimum_required(VERSION 3.17)
project(Mandelbrot)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(Mandelbrot main.cpp Mandelbrot.h Julia.cpp Julia.h Mandelbrot.cpp RandomString.h)

target_link_libraries(Mandelbrot sfml-graphics sfml-audio sfml-system sfml-window)