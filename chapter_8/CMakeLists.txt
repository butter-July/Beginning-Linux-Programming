cmake_minimum_required(VERSION 3.10)
project(chapter_8 C)

set(CMAKE_C_STANDARD 11)

# 查找 MySQL 库
include_directories(/usr/include/mysql)
link_directories(/usr/lib/x86_64-linux-gnu)

add_executable(chapter_8 connect1.c
        connect2.c
        insert1.c
        insert2.c
        select.c
        test.c
)

# 链接 MySQL 客户端库
target_link_libraries(chapter_8 mysqlclient)
