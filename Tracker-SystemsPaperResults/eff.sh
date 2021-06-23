#!/bin/bash

for i in `ls *.json`; do
    echo $i
    cat $i | python -m json.tool | grep "straight_efficiency"
    
done

for i in `ls *.json`; do
    echo $i
    cat $i | python -m json.tool | grep "helical_efficiency"
    
done

