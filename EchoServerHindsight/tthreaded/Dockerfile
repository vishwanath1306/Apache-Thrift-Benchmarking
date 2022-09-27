FROM golang

ARG NUM_CPUS=20

ARG DEBIAN_FRONTEND=noninteractive

COPY ./ /echohindsight-tthreaded

ARG SSH_PRIVATE_KEY


RUN apt-get update \
    && apt-get install -y automake bison flex g++ git libboost-all-dev \
    libevent-dev libssl-dev libtool make pkg-config supervisor

# Installing Apache Thrift
RUN git clone https://github.com/apache/thrift.git ; \
    cd thrift ; \
    ./bootstrap.sh ; \
    ./configure ; \
    make -j4 ; \
    make install ; \
    ldconfig ; 

# Installing Hindsight

## Adding SSH keys for accessing private repo
RUN mkdir /root/.ssh/
RUN echo "${SSH_PRIVATE_KEY}" > /root/.ssh/id_rsa
RUN chmod 600 /root/.ssh/id_rsa
RUN touch /root/.ssh/known_hosts
RUN ssh-keyscan github.com >> /root/.ssh/known_hosts

# Actual hindsight install
RUN mkdir /hindsight
WORKDIR /hindsight
RUN git clone git@github.com:SimBioSysLab/hindsight.git ; \
    cd hindsight/client ; \
    make && make install; ldconfig ;\
    cd ../agent && go build -o agent cmd/agent2/main.go; \
    mv agent /usr/local/bin/ ;

WORKDIR /echohindsight-tthreaded
RUN cp echotthreaded.conf /etc/hindsight_conf; \
    cp echottdhs_supervisor.conf /etc/echottdhs_supervisor.conf; \
    make server.o && cp server.o /etc/server.o

EXPOSE 5050
EXPOSE 3046

CMD [ "supervisord", "-c", "/etc/echottdhs_supervisor.conf" ]


# docker run --rm -it  -p 3046:3046/tcp -p 5252:5252/tcp --shm-size 8G echottdhs:latest