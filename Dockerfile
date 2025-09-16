FROM ubuntu:24.04

# 1. install tools
# 2. install llvm dependencies
# 3. install project dependencies
RUN apt-get update -y &&\
    apt-get install -y g++ cmake &&\
    apt-get install -y lsb-release wget software-properties-common gnupg &&\
    apt-get install -y libgtkmm-3.0-dev libcairomm-1.0-dev libglibmm-2.4-dev &&\
    apt-get clean &&\
    rm -rf /var/lib/apt/lists/*

RUN wget https://apt.llvm.org/llvm.sh &&\
    chmod u+x llvm.sh &&\
    ./llvm.sh 21 &&\
    update-alternatives --install /usr/bin/clang clang /usr/bin/clang-21 100 &&\
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-21 100 &&\
    update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-21 100

USER ubuntu

WORKDIR /project
