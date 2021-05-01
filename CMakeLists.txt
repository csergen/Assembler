cmake_minimum_required(VERSION 3.5)
project(Assembler C)

set(CMAKE_C_STANDARD 11)

include_directories(init)
include_directories(init/node)
include_directories(init/parser)
include_directories(init/tokenizer)
include_directories(init/vm)
include_directories(init/util)
include_directories(init/assemble)

set (SRCS
    assembler.c
    init/args.h
    init/util/util.h
    init/util/util.c
    init/tokenizer/tokenizer.h
    init/tokenizer/tokenizer.c
    init/parser/parser.h
    init/parser/parser.c
    init/node/node.h
    init/node/node.c
    init/vm/vm.h
    init/vm/vm.c
    init/assemble/assemble.h
    init/assemble/assemble.c)

add_executable(Assembler ${SRCS})
target_link_libraries(Assembler m)