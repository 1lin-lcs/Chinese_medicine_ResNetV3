import torch
from MobileNetV3 import *
from torchvision import transforms
from PIL import Image
import json
import os

transform=transforms.Compose([
        transforms.Resize((224,224)),
        transforms.ToTensor(),
        transforms.Normalize([0.435, 0.497, 0.328],[0.264, 0.253, 0.276])
    ])

device=torch.device("cuda" if torch.cuda.is_available() else "cpu")

def testimg(img_path,path):
    file_path=os.path.join(path,"data","Chinese_Medicine_for_test.json")
    model_path=os.path.join(path,"data","ModelParm.pth")
    with open(file_path,'r') as f:
        medicine=json.load(f)
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
        return str(medicine[str(out)])
    except Exception as e:
        return str(e)
