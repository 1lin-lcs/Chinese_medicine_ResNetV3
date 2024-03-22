#!/bin/bash

files=$(ls)

for file in ${files[*]};do
        array=(${file//_/ })
        #for ((i=0;i<${#array[*]};i+=3));do
        echo ${array[0]}_${array[1]}
        mv ${array[0]}_${array[1]} ${array[0]}F_${array[1]}
        #done
done        
