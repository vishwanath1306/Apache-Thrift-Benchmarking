# Docker Build commands

For building docker image
```
docker build -t echottdhs --build-arg SSH_PRIVATE_KEY="$(cat ~/.ssh/id_rsa)" .
```

Deploying Docker container

```
docker run --rm -it  -p 3046:3046/tcp -p 5252:5252/tcp --shm-size 8G echottdhs:latest
```