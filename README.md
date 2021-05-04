# Assembler
[![CMake](https://github.com/csergen/Assembler/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/csergen/Assembler/actions/workflows/cmake.yml)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/a0630c3f393d44f0be60b17416025324)](https://www.codacy.com/gh/csergen/Assembler/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=csergen/Assembler&amp;utm_campaign=Badge_Grade)

[![csergen](https://circleci.com/gh/csergen/Assembler.svg?style=svg)](https://app.circleci.com/pipelines/github/csergen/Assembler?invite=true)


![C Badge](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Fedora Badge](https://img.shields.io/badge/Fedora-294172?style=for-the-badge&logo=fedora&logoColor=white)
![Ubuntu Badge](https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)

## Machine
![machine](docs/img/final.jpg)


## build with make
```console
$ make

$ make run

$ make clean
```

## build with cmake
```console
$ cmake -S . -B build/Debug -D CMAKE_BUILD_TYPE=Release
$ cd build/Debug
$ ./Assembler <source_file_path>
```
## build with docker
```xml
$ sudo systemctl start docker
$ sudo docker build -t <custom_image_name> <dockerfile_path>
$ sudo docker run -it --rm --name <custom_container_name> <custom_image_name>
```



## EXAMPLES

### Factorial
![factorial](docs/img/factorial.gif)

```assembly
RN: 5

HRK AX, [RN]
HRK CX, AX

FACT: 
      CIK AX, 1
      SS END
      CRP CX, AX
      SP FACT

END:
```

### Fibonacci
![fibonacci](docs/img/fibonacci.gif)

```assembly
x1: 1
x2: 1

HRK AX, [x1]
HRK BX, [x2]

FIB:
  HRK CX, AX
  TOP AX, BX
  HRK DX, AX
  HRK BX, CX
  HRK AX, DX
  SP FIB
```

### Counter Example
![ex](docs/img/ex.gif)

```assembly
co: 1

HRK AX, 10
HRK BX, 0

HRK CX, AX
DEG CX
TOP CX, 1

LOP:
  TOP BX, [co]
  TOP CX, [co]
  SSD LOP
  SS DEC

DEC:
  CIK BX, [co]
  SP DEC
```
