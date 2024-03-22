#!/bin/bash

echo "输入文件夹名字"
read dir
#path=$(pwd)
src_path="/mnt/e/Downloads/0202整理/"
des_path="/mnt/e/文件/Documents/Python/pytorch_learning/中草药/"
suffix=".png"

for d in ${dir[*]};do        
    files=($(ls $src_path$d))
    echo $src_path$d
    
    num=0
    for ((i=0;i<${#files[*]};i+=3));do
        num=$[$num+1]
        cp ${src_path}${d}"/"${files[i]}\ ${files[i+1]}\ ${files[i+2]} ${des_path}${d}"/"${d}_${num}${suffix}
        echo ${des_path}${d}"/"${d}_${num}${suffix}
    done
done
