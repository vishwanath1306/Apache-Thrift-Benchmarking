
FROM yg397/thrift-microservice-deps:xenial

ARG NUM_CPUS=4

COPY ./ /thrift-tutorial
RUN cd /thrift-tutorial \
    && make 

CMD ["./server"]

WORKDIR /thrift-tutorial