FROM ubuntu

ARG NUM_CPUS=4

ARG DEBIAN_FRONTEND=noninteractive

COPY ./ /uniqueid-threadpool
RUN apt-get update \
    && apt-get install -y automake bison flex g++ git libboost-all-dev libevent-dev libssl-dev libtool make pkg-config

RUN git clone https://github.com/apache/thrift.git ; \
    cd thrift ; \
    ./bootstrap.sh ; \
    ./configure --with-cpp=yes --with-c_glib=no --with-java=no --with-ruby=no --with-erlang=no --with-go=no --with-nodejs=no ; \
    make -j4 ; \
    make install ; \
    ldconfig ; 

RUN cd /uniqueid-threadpool ; \
    make server.o

CMD ["./server.o"]

WORKDIR /uniqueid-threadpool
