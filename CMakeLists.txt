cmake_minimum_required(VERSION 3.5)
project(Assembler C)

set(CMAKE_C_STANDARD 11)

include_directories(init)

set (SRCS
    assembler.c
    init/args.h
    init/util.c
    init/util.h
    init/tokenizer.c
    init/tokenizer.h
    init/parser.c
    init/parser.h
    init/node.h
    init/node.c
    init/vm.h
    init/vm.c)

add_executable(Assembler ${SRCS})