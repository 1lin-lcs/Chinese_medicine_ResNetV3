import torchvision
import torch

vgg16=torchvision.models.vgg16(pretrained=True)

torch.save(vgg16,"")#path  func1   ����ģ�ͽṹ�Ͳ���    ���ǻ�Ҫ����ģ�͵�class

torch.save(vgg16.state_dict(),"")#path   func2   ����ģ�Ͳ���

torch.load()#path ��Ӧfunc1

vgg16.load_state_dict(torch.load())#path ��Ӧfunc2
