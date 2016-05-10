FROM ubuntu:16.04

RUN apt-get update \
 && apt-get install -y \
    cmake \
    make \
    g++ \
    libclang-3.6-dev \
    liblldb-3.6-dev \
    clang-format-3.6 \
    pkg-config \
    libboost-filesystem-dev \
    libboost-regex-dev \
    libgtksourceviewmm-3.0-dev \
    aspell-en \
    libaspell-dev \
 && rm -rf /var/lib/apt/lists/*

ENV make_args ""
ENV cmake_args "-DCMAKE_CXX_COMPILER=g++"

CMD make ${make_args}
