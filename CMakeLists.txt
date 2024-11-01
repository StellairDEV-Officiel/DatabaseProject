cmake_minimum_required(VERSION 3.22.1)
project(DatabaseProject C)

set(CMAKE_C_STANDARD 11)

add_executable(DatabaseProject src/main.c
        src/command.c
        include/command.h
        src/table.c
        include/table.h
        src/record.c
        include/record.h
)
