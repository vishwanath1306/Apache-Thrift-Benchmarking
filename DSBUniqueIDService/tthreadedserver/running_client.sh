#!/bin/bash

for i in {1..100}
do  
    echo $i
    curl http://localhost:9080
done