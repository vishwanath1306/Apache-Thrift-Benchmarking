#!/bin/bash

for i in {1..700}
do  
    echo $i
    ./client.o 120&
done