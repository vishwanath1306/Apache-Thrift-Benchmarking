
# FROM yg397/thrift-microservice-deps:xenial

# ARG NUM_CPUS=4

# COPY ./ /compute-ttd
# RUN cd /compute-ttd \
#     && make server.o 

# CMD ["./server.o"]

# WORKDIR /compute-ttd

FROM ubuntu:18.04
ARG NUM_CPUS=4

RUN apt-get update && apt-get install -y --no-install-recommends \
  build-essential \
  ca-certificates \
  cmake \
  curl \
  git \
  libcurl4-openssl-dev \
  libgflags-dev \
  unzip

WORKDIR /
RUN apt-get update && apt-get install -y \
    libboost-dev \
    libboost-test-dev \
    libboost-program-options-dev \
    libboost-filesystem-dev \
    libboost-thread-dev \
    libevent-dev \
    automake \
    libtool \
    flex \
    bison \
    pkg-config \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /
RUN git clone https://github.com/apache/thrift.git ; \
    cd thrift ; \
    ./bootstrap.sh ; \
    ./configure --with-cpp=yes --with-c_glib=no --with-java=no --with-ruby=no --with-erlang=no --with-go=no --with-nodejs=no ; \
    make -j4 ; \
    make install ; \
    ldconfig ; 

WORKDIR /
RUN curl https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.8.0%2Bcpu.zip --output libtorch.zip \
    && unzip libtorch.zip \
    && rm libtorch.zip

COPY ./ /mlinfer-ttd
RUN cd /mlinfer-ttd \
    && make server.o
ENV LD_LIBRARY_PATH /libtorch/lib:/usr/local/lib
ENTRYPOINT [ "./server.o" ]
WORKDIR /mlinfer-ttd