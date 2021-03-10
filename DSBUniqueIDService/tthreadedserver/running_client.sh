#!/bin/bash

for i in {1..1000}
do  
    echo $i
    ./client.o 120&
done