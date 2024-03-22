import torchvision
import torch

vgg16=torchvision.models.vgg16(pretrained=True)

torch.save(vgg16,"")#path  func1   保存模型结构和参数    但是还要保存模型的class

torch.save(vgg16.state_dict(),"")#path   func2   保存模型参数

torch.load()#path 对应func1

vgg16.load_state_dict(torch.load())#path 对应func2
