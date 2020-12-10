
FROM yg397/thrift-microservice-deps:xenial

ARG NUM_CPUS=4

COPY ./ /echo-service
RUN cd /echo-service \
    && make 

CMD ["./server"]

WORKDIR /echo-service