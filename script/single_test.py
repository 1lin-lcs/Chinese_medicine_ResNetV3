import torch
from MobileNetV3 import *
from torchvision import transforms
from PIL import Image
import os
import json

############################################
#file_path是类别对应文件的文件路径             #
#model_path是模型参数文件路径  			   #
#img_path是图片文件的路径（可选）			   #
#testimg是检测单个图片的函数				   #
############################################

file_path=r"../Chinese_Medicine_for_test.json"
with open(file_path,'r') as f:
    medicine=json.load(f)

def testimg(img_path):
    try:
        img=Image.open(img_path)
        img=transform(img)
        img=torch.reshape(img,(-1,3,224,224))
        img=img.to(device)
    
        model=torch.load(model_path)
        model=model.to(device)

        with torch.no_grad():
            output=model(img)

        out=output.argmax(1).item()
        print(medicine[str(out)])
        #print(output)
    except Exception as e:
        print(e)

device=torch.device("cuda" if torch.cuda.is_available() else "cpu")

transform=transforms.Compose([
        transforms.Resize((224,224)),
        transforms.ToTensor(),
        transforms.Normalize([0.435, 0.497, 0.328],[0.264, 0.253, 0.276])
    ])

model_path=r"../example_small_1.pth"

#可以只使用函数，对单个图片进行检测
'''
img_path=r""
files=os.listdir(img_path)
a=0
for i in files:
    file=os.path.join(img_path,i)
    print(a,end='\t')
    testimg(file)
    a+=1
'''