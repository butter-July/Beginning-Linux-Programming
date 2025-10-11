cmake_minimum_required(VERSION 4.0)
project(cd_data C)

set(CMAKE_C_STANDARD 11)

add_executable(cd_data app_ui.c
        cd_data.h
        cd_access.c)
