FROM ubuntu:18.04

RUN apt-get update -y && apt-get install -y --no-install-recommends  \
                    gcc                         \
                    g++                         \
                    make                        \
                    build-essential             \
                    cmake   &&                  \
    apt-get autoclean       &&                  \
    apt-get autoremove      &&                  \
    apt-get clean           &&                  \
    rm -rf /var/lib/apt/lists/*

COPY . /usr/src/assembler
RUN mkdir "mkdir /usr/src/assembler/build"
WORKDIR /usr/src/assembler/build
RUN cmake ..
RUN make
CMD ["./Assembler", "../example/1.asm"]