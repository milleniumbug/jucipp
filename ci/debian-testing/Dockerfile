FROM debian:testing

RUN apt-get update \
 && apt-get install -y --fix-missing \
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

ENV make_args "-j 2"
ENV cmake_args "-DCMAKE_CXX_COMPILER=g++"
