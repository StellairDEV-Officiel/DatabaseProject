cmake_minimum_required(VERSION 3.22.1)
project(DatabaseProject C)

set(CMAKE_C_STANDARD 11)

add_executable(DatabaseProject src/main.c
        src/command.c
        include/command.h
        src/records_btree.c
        include/records_btree.h
        src/table_btree.c
        include/table_btree.h
        src/storage.c
        include/storage.h
        src/database.c
        include/database.h
)
