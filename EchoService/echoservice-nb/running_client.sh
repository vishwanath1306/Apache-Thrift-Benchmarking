#!/bin/bash

for i in {1..10000}
do
    echo $i
    ./client.o &
done