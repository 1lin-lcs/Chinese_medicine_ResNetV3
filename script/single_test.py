import torch
from MobileNetV3 import *
from torchvision import transforms
from PIL import Image
import os
import json

file_path=r"E:\文件\Documents\Python\pytorch_learning\Chinese_Medicine_old.json"
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

model_path=r"E:\文件\Documents\Python\pytorch_learning\bak\small\4\test42_acc0.7186858654022217.pth"
img_path=r"E:\文件\Documents\Python\pytorch_learning\中草药\Agastacherugosa"

files=os.listdir(img_path)
a=0
for i in files:
    file=os.path.join(img_path,i)
    print(a,end='\t')
    testimg(file)
    a+=1
