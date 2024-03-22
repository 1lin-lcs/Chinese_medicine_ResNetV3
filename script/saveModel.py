import torch
import torchvision
#from MobileNetV3 import *
from ResNet import *

device=torch.device("cuda" if torch.cuda.is_available() else "cpu")

if __name__=='__main__':
    model=torch.load(r"E:\文件\Documents\Python\pytorch_learning\bak\test\ACC_18_843.pth")
    model=model.to(device)
    example=torch.rand(1,3,224,224)
    example=example.to(device)
    traces_script_module=torch.jit.script(model)
    traces_script_module.save(r"E://ACC_843.pt")