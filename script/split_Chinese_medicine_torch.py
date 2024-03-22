import torch
from torchvision import datasets
import os
from PIL import Image
import warnings
import math

warnings.filterwarnings("error",category=UserWarning)

root_dir=r"E:\文件\Documents\Python\pytorch_learning\中草药"
root_path=r"E:\文件\Documents\Python\pytorch_learning\CM_Ndata"
if not os.path.exists(root_path):
    os.mkdir(root_path)
dataset=datasets.ImageFolder(root_dir)

dataset.classes
dataset.class_to_idx
with open("Record.txt","w") as f:
    f.write(str(dataset.class_to_idx))

data=[]

for file in dataset.imgs:
    data.append(file)

data_len=len(data)
#print(data_len)

train_size=math.ceil(0.6*data_len)
test_size=int(0.2*data_len)
valid_size=int(0.2*data_len)

def split_imgs(classes,file):
    if not os.path.exists(os.path.join(root_path,"train")):
        os.mkdir(os.path.join(root_path,"train"))
    if not os.path.exists(os.path.join(root_path,"test")):
        os.mkdir(os.path.join(root_path,"test"))
    if not os.path.exists(os.path.join(root_path,"valid")):
        os.mkdir(os.path.join(root_path,"valid"))

    if classes=="train":
        path=os.path.join(root_path,"train")
    elif classes=="test":
        path=os.path.join(root_path,"test")
    elif classes=="valid":
        path=os.path.join(root_path,"valid")
    else:
        print("agr1 error!")
        return
    file_name=os.path.basename(file)
    file_classes=file_name.split("_")[0]
    path=os.path.join(path,file_classes)
    if not os.path.exists(path):
        os.mkdir(path)
    try:
        img=Image.open(file)
        if img.mode=="P":
            img=img.convert("RGB")
        out=img.resize((224,224),Image.Resampling.LANCZOS)
        out.save(path+"\\"+file_name)
    except:
        print(file)

train_datasets,test_datasets,valid_datasets=torch.utils.data.random_split(data,[train_size,test_size,valid_size])

train_imgList_line=[]

os.chdir(root_path)

for line in train_datasets:
    train_imgList_line.append(line[0])
    split_imgs("train",line[0])
    train_imgList_line.append(line[1])

with open("./train.txt","wt") as f:
    i=0
    for line in train_imgList_line:
        if i==train_size:
            break
        if i%2==0:
            f.write(str(line)+"\n")
        else:
            f.write(str(line)+"\n")
        i=+1

test_imgList_line=[]

for line in test_datasets:
    test_imgList_line.append(line[0])
    split_imgs("test",line[0])
    test_imgList_line.append(line[1])

with open("./test.txt","wt") as f:
    i=0
    for line in test_imgList_line:
        if i==test_size:
            break
        if i%2==0:
            f.write(str(line)+"\n")
        else:
            f.write(str(line)+"\n")
        i=+1

valid_imgList_line=[]

for line in valid_datasets:
    valid_imgList_line.append(line[0])
    split_imgs("valid",line[0])
    valid_imgList_line.append(line[1])

with open("./valid.txt","wt") as f:
    i=0
    for line in valid_imgList_line:
        if i==valid_size:
            break
        if i%2==0:
            f.write(str(line)+"\n")
        else:
            f.write(str(line)+"\n")
        i=+1