FROM golang

ARG NUM_CPUS=20

ARG DEBIAN_FRONTEND=noninteractive

COPY ./ /echohindsight-tthreaded

ARG SSH_PRIVATE_KEY


RUN apt-get update \
    && apt-get install -y automake bison flex g++ git libboost-all-dev \
    libevent-dev libssl-dev libtool make pkg-config 

# Installing Apache Thrift
RUN git clone https://github.com/apache/thrift.git ; \
    cd thrift ; \
    ./bootstrap.sh ; \
    ./configure --with-cpp=yes --with-c_glib=no --with-java=no --with-ruby=no --with-erlang=no --with-go=no --with-nodejs=no ; \
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
RUN git clone git@github.com:SimBioSysLab/hindsight.git \
    && 

CMD [ "echo", "Hello World" ]


