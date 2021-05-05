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
WORKDIR /usr/src/assembler/build
RUN cmake ..
RUN make
WORKDIR ../
CMD ["./build/Assembler"]
#CMD ["./build/Assembler", "../example/branch.asm"]
