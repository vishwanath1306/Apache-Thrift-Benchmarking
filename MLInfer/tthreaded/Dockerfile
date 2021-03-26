
FROM yg397/thrift-microservice-deps:xenial

ARG NUM_CPUS=4

COPY ./ /compute-ttd
RUN cd /compute-ttd \
    && make server.o 

CMD ["./server.o"]

WORKDIR /compute-ttd