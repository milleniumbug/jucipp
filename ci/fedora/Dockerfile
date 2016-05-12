FROM fedora:23

RUN dnf -y update \
 && dnf -y install \
    cmake \
    make \
    gcc-c++ \
    clang-devel \
    clang \
    lldb-devel \
    boost-devel \
    gtksourceviewmm3-devel \
    gtkmm30-devel \
    aspell-devel \
    aspell-en \
 && dnf clean all

ENV make_args "-j 2"
ENV cmake_args "-DCMAKE_CXX_COMPILER=g++"
