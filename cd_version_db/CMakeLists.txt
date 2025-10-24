cmake_minimum_required(VERSION 4.0)
project(cd_db_version C)

set(CMAKE_C_STANDARD 11)

add_executable(cd_db_version
        app_mysql.h
        app_mysql.c
        main.c
)
