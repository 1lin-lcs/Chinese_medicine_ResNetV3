import csv
from genericpath import isfile
from inspect import getfile
import torch
import os
from torchvision import transforms
from PIL import Image
import json
from ResNet import *

device=torch.device("cuda" if torch.cuda.is_available() else "cpu")

categoryNum=59

normMean = [0.44851533, 0.49718806, 0.33081618]
normStd = [0.26867545, 0.25197837, 0.28146]

trans=transforms.Compose(
    [
            transforms.Resize((224,224)),
            transforms.ToTensor(),
            transforms.Normalize(normMean,normStd)
    ]
    )

def getFiles(path):
    if not os.path.exists(path):
        raise NameError("目录不存在")
    if not os.path.isdir(path):
        raise NameError("不是目录")
    files=os.listdir(path)
    return path,files

def test_every_pic_result(path,files,transform,jsonFile,model,resultFile):
    if not os.path.isfile(jsonFile) or jsonFile.split(".")[-1]!="json":
        raise NameError("不是Json文件")
    jsonfile=open(jsonFile,'r')
    Names=json.load(jsonfile)
    results=[0]*categoryNum
    for f in files:
        image=Image.open(os.path.join(path,f))
        image=image.convert("RGB")
        image=transform(image)
        image=image.unsqueeze(0)
        image=image.to(device)
        pre_lab=torch.argmax(model(image),dim=1)
        #print(pre_lab.item())
        results[pre_lab.item()]+=1
    csvfile=open(resultFile,'w')
    for i in range(0,len(results)):
        csvfile.write(Names.get(str(i))+","+str(results[i])+'\n')
    csvfile.close()
    jsonfile.close()
    
if __name__=="__main__":
    path=r"E:\文件\Documents\Python\pytorch_learning\CM_Ndata\test"
    #path=r"E:\文件\Documents\Python\pytorch_learning\中草药"
    result_path=r"E:\文件\Documents\Python\pytorch_learning\AnalyzeData"
    jsonFile=r"E:\文件\Documents\Python\pytorch_learning\Chinese_Medicine_N.json"
    #modelFile=r"E:\文件\Documents\Python\pytorch_learning\bak\test\ACC_18_833.pth"
    modelFile=r"E:\文件\Documents\Python\pytorch_learning\bak\test\ACC_18_843.pth"
    category="feipeng"
    suffix=""
    model=torch.load(modelFile).to(device)
    path,files=getFiles(os.path.join(path,category))
    if suffix!="":
        category=category+suffix
    test_every_pic_result(path,files,trans,jsonFile,model,os.path.join(result_path,category+".csv"))
    