#!/bin/bash

for i in {1..500}
do  
    echo $i
    ./client.o 120&
done