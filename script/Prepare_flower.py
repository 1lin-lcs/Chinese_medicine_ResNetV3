import os
import numpy as np
import pandas as pd
import scipy
from scipy import io
from PIL import Image
import shutil

labels=scipy.io.loadmat(r"E:\文件\Documents\Python\pytorch_learning\Flowers\label\imagelabels.mat")
labels=np.array(labels['labels'][0])-1
setid=scipy.io.loadmat(r"E:\文件\Documents\Python\pytorch_learning\Flowers\label\setid.mat")
validation=np.array(setid['valid'][0])-1
np.random.shuffle(validation)
train=np.array(setid['trnid'][0])-1
np.random.shuffle(train)
test=np.array(setid['tstid'][0])-1
np.random.shuffle(test)
flower_dir=list()

for img in os.listdir(r"E:\文件\Documents\Python\pytorch_learning\Flowers\jpg"):
    flower_dir.append(os.path.join(r"E:\文件\Documents\Python\pytorch_learning\Flowers\jpg",img))
flower_dir.sort()

#训练集
des_folder_train=r"E:\文件\Documents\Python\pytorch_learning\Flowers\train"
for tid in train:
    img=Image.open(flower_dir[tid])
    img=img.resize((224,224),Image.Resampling.LANCZOS)
    label=labels[tid]
    path=flower_dir[tid]
    base_path=os.path.basename(path)    #返回path最后的文件名
    classes="c"+str(label)
    class_path=os.path.join(des_folder_train,classes)
    if not os.path.exists(class_path):
        os.makedirs(class_path)
    despath=os.path.join(class_path,base_path)
    img.save(despath)

#验证集
des_folder_validation=r"E:\文件\Documents\Python\pytorch_learning\Flowers\validation"
for tid in validation:
    img=Image.open(flower_dir[tid])
    img=img.resize((224,224),Image.Resampling.LANCZOS)
    label=labels[tid]
    path=flower_dir[tid]
    base_path=os.path.basename(path)
    classes="c"+str(label)
    class_path=os.path.join(des_folder_validation,classes)
    if not os.path.exists(class_path):
        os.makedirs(class_path)
    despath=os.path.join(class_path,base_path)
    img.save(despath)

#测试集
des_folder_test=r"E:\文件\Documents\Python\pytorch_learning\Flowers\test"
for tid in test:
    img=Image.open(flower_dir[tid])
    img=img.resize((224,224),Image.Resampling.LANCZOS)
    label=labels[tid]
    path=flower_dir[tid]
    base_path=os.path.basename(path)
    classes="c"+str(label)
    class_path=os.path.join(des_folder_test,classes)
    if not os.path.exists(class_path):
        os.makedirs(class_path)
    despath=os.path.join(class_path,base_path)
    img.save(despath)