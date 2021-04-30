FROM ubuntu:latest

RUN apt-get update -y && \
    apt-get install -y tzdata

RUN apt-get install -y --no-install-recommends  \
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
RUN mkdir "mkdir build"
WORKDIR ./usr/src/assembler/build
RUN cmake ..
RUN make
CMD ["./Assembler"]