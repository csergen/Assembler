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
	init/tokenizer.h)

add_executable(Assembler ${SRCS})
