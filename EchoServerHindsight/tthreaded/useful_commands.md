# Docker Build commands

For building base docker image
```
docker build -t thrift-hindsight --build-arg SSH_PRIVATE_KEY="$(cat ~/.ssh/id_rsa)" .
```

For building the server with Hindsight agent
```
docker build -f Dockerfile_server -t echottdhs .
```

Deploying Docker container

```
docker run --rm -it  -p 3046:3046/tcp -p 5252:5252/tcp --shm-size 8G echottdhs:latest
```