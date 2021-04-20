cmake_minimum_required(VERSION 3.17)
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
        init/node.c)

add_executable(Assembler ${SRCS})

enable_testing()

add_executable(test_init_util_open_stream 
    test/test_init_util_open_stream.c)

add_executable(test_init_util_close_stream 
    test/test_init_util_close_stream.c)

add_test(NAME
            unit_test
         COMMAND ${CMAKE_COMMAND}
            -DCMD1=$<TARGET_FILE:test_init_util_open_stream>
            -DCMD2=$<TARGET_FILE:test_init_util_close_stream>
         -P ${CMAKE_CURRENT_SOURCE_DIR}/runtests.cmake)
