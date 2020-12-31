FROM yg397/openresty-thrift:xenial

# RUN apt-get update \
#     && apt-get install -y automake bison vim flex g++ git libboost-all-dev libevent-dev libssl-dev libtool make pkg-config

# RUN git clone https://github.com/apache/thrift.git ; \
#     cd thrift ; \
#     ./bootstrap.sh ; \
#     ./configure --with-cpp=yes --with-c_glib=no --with-java=no --with-ruby=no --with-erlang=no --with-go=no --with-nodejs=no --with--lua=yes; \
#     make -j4 ; \
#     make install ; \
#     ldconfig ; 

COPY ./nginx.conf /usr/local/openresty/nginx/conf/nginx.conf
COPY ./app.lua /etc/nginx/lua-scripts/app.lua
COPY ./gen-lua /gen-lua

# WORKDIR /sample-openresty
