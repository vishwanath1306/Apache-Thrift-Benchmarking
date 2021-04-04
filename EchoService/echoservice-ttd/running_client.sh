#!/bin/bash

for i in {1..100}
do  
    echo $i
    ./client.o 60&
done