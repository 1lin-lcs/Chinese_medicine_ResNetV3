#!/bin/bash

path="xiaoji1"
path1="xiaoji1"
name="xiaoji"
number=0
files=$(ls ./${path})

if [ ! -d "${path1}" ];then
        mkdir "${path1}"
fi

for file in ${files[*]};do
        if [ ${number} -lt 10 ];then
            cp ./${path}/$file ./${path1}/${name}_00${number}.jpg
        elif [ ${number} -lt 100 ] && [ ${number} -gt 10 ];then
            cp ./${path}/$file ./${path1}/${name}_0${number}.jpg
        else
            cp ./${path}/$file ./${path1}/${name}_${number}.jpg
        fi
        number=$[ $number + 1 ]
done       
