cmake_minimum_required(VERSION 3.5)
project(Assembler C)

set(CMAKE_C_STANDARD 11)

include_directories(core)
include_directories(core/node)
include_directories(core/parser)
include_directories(core/tokenizer)
include_directories(core/vm)
include_directories(core/util)
include_directories(core/assemble)

set (SRCS
    core/assembler.c
    core/args.h
    core/util/util.h
    core/util/util.c
    core/tokenizer/tokenizer.h
    core/tokenizer/tokenizer.c
    core/parser/parser.h
    core/parser/parser.c
    core/node/node.h
    core/node/node.c
    core/vm/vm.h
    core/vm/vm.c
    core/assemble/assemble.h
    core/assemble/assemble.c)

add_executable(Assembler ${SRCS})
target_link_libraries(Assembler m)