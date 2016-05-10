FROM debian:jessie

RUN apt-get update \
 && apt-get install -y --fix-missing \
    cmake \
    make \
    g++ \
    libclang-3.5-dev \
    liblldb-3.5-dev \
    clang-format-3.5 \
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