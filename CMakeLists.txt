cmake_minimum_required(VERSION 3.5)
project(Assembler C)

set(CMAKE_C_STANDARD 11)

include_directories(core)
include_directories(core/Node)
include_directories(core/Parser)
include_directories(core/Tokenizer)
include_directories(core/Vm)
include_directories(core/Util)
include_directories(core/Assemble)

set (SRCS
    core/assembler.c
    core/args.h
    core/Util/util.h
    core/Util/util.c
    core/Tokenizer/tokenizer.h
    core/Tokenizer/tokenizer.c
    core/Parser/parser.h
    core/Parser/parser.c
    core/Node/node.h
    core/Node/node.c
    core/Vm/vm.h
    core/Vm/vm.c
    core/Assemble/assemble.h
    core/Assemble/assemble.c)

add_executable(Assembler ${SRCS})
target_link_libraries(Assembler m)