FROM gcc:4.9


# RUN wget https://github.com/Kitware/CMake/releases/download/v3.14.0-rc3/cmake-3.14.0-rc3.tar.gz && \
#     tar xvfz cmake-3.14.0-rc3.tar.gz && \
#     cd cmake-3.14.0-rc3 && \
#     ./configure && make && make install && \
#     cd / && rm -rf ./cmake*
    
RUN apt-get update && \
    apt-get -y install cmake libgtest-dev valgrind valgrind-dbg valgrind-mpi valkyrie
    