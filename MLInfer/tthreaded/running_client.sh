#!/bin/bash

for i in {1..50}
do  
    echo $i
    curl http://localhost:9080 &
done