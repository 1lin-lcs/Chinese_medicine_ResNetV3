import torch
import torchvision
from torch.utils.data import DataLoader
from torch import nn
from torch.nn import Conv2d
from tensorboardX import SummaryWriter

dataset=torchvision.datasets.CIFAR10(root="./cifar10",train=False,transform=torchvision.transforms.ToTensor(),download=True)

dataloader=DataLoader(dataset,batch_size=64)

class Test(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1=Conv2d(in_channels=3,out_channels=6,kernel_size=2,stride=1,padding=0)

    def forward(self,input):
        x=self.conv1(input)
        return x

t=Test()
a=0
writer=SummaryWriter("log")
for data in dataloader:
    imgs,targets=data
    output=t(imgs)
    writer.add_images("input",imgs,a)
    output=torch.reshape(output,(-1,3,31,31))
    writer.add_images("output",output,a)
    a+=1
    if(a>100):
        break