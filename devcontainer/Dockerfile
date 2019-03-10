FROM gcc:latest


# RUN wget https://github.com/Kitware/CMake/releases/download/v3.14.0-rc3/cmake-3.14.0-rc3.tar.gz && \
#     tar xvfz cmake-3.14.0-rc3.tar.gz && \
#     cd cmake-3.14.0-rc3 && \
#     ./configure && make && make install && \
#     cd / && rm -rf ./cmake* && \
RUN wget https://github.com/Kitware/CMake/releases/download/v3.13.4/cmake-3.13.4-Linux-x86_64.sh  && \
    mkdir /opt/cmake && \
    sh /cmake-3.13.4-Linux-x86_64.sh --prefix=/opt/cmake --skip-license && \
    ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake && \
    cmake --version

# RUN apt-get update && \
#     apt-get -y install cmake valgrind valgrind-dbg valgrind-mpi valkyrie less nano 
    