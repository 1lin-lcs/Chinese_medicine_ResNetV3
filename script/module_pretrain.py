import torchvision
from torch.utils.data import DataLoader
from torch.nn import *

vgg16_false=torchvision.models.vgg16(pretrianed=False)
vgg16_true=torchvision.models.vgg16(pretrianed=True)

dataset=torchvision.datasets.CIFAR10(root="./cifar10",train=False,transform=torchvision.transforms.ToTensor(),download=True)
dataloader=DataLoader(dataset=dataset,batch_size=64)

vgg16_true.add_module('add_linear',Linear(1000,10)) #Ìí¼Ó
vgg16_false.classifier[6]=Linear(4096,10)   #ÐÞ¸Ä