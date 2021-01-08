#!/bin/bash

for i in {1..1000}
do  
    echo $i
    ./client.o &
    sleep 0.001
done