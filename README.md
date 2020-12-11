# Apache Thrift Services for Benchmarking

## Running the EchoTThreadedServer

For running the server

```

cd echoservice-ttd
docker build -t hellowrold .
docker run --publish 3062:3062 --detach --name hw helloworld:latest
```

For running the client
```
cd echoservice-ttd
make client
./client

```

## Running the EchoNonBlockingServer

For running the server 

```
cd echoservice-nb
docker build -t nbecho .
docker run --rm -it -p 3062:3062 -d nbecho:latest
```

For running the client
```
cd echoservice-nb
make client
./client
```
