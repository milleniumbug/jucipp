FROM codekoala/arch

RUN pacman -Syuu --noconfirm \
 && pacman -S --noconfirm \
    cmake \
    pkg-config \
    make \
    clang \
    gtksourceviewmm \
    boost \
    aspell \
    aspell-en

ENV make_args ""
ENV cmake_args "-DCMAKE_CXX_COMPILER=g++"

CMD make ${make_args}
