cmake_minimum_required(VERSION 3.29)
project(image_processing)

set(CMAKE_CXX_STANDARD 20)

add_executable(image_processing main.cpp
        logging.h
        png.cpp
        png.h
        ppm.cpp
        ppm.h
        color.h
        color.cpp
        image_process.cpp
        image_process.h
        image_generator.h)
