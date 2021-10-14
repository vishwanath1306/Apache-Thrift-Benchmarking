FROM ubuntu:18.04
ARG NUM_CPUS=16

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
    make -j{NUM_CPUS} ; \
    make install ; \
    ldconfig ; 

COPY ./ /uniqueid-async-tp
RUN cd /uniqueid-async-tp \
    && make server.o
ENTRYPOINT [ "./server.o" ]
WORKDIR /uniqueid-async-tp