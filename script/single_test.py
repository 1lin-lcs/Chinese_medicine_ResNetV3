import torch
from MobileNetV3 import *
from torchvision import transforms
from PIL import Image
import os
import json

file_path=r"E:\文件\Documents\Python\pytorch_learning\Chinese_Medicine_for_test.json"
with open(file_path,'r') as f:
    medicine=json.load(f)

def testimg(img_path):
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

device=torch.device("cuda" if torch.cuda.is_available() else "cpu")

transform=transforms.Compose([
        transforms.Resize((224,224)),
        transforms.ToTensor()
    ])

model_path=r"E:\文件\Documents\Python\pytorch_learning\bak\small\test46_acc0.4932238459587097.pth"
img_path=r"E:\文件\Documents\Python\pytorch_learning\中草药\cangerzi"

files=os.listdir(img_path)
for i in files:
    file=os.path.join(img_path,i)
    testimg(file)
