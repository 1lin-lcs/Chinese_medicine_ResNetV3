from telnetlib import OUTMRK
import torch
from torch import nn
from torch.nn import ReLU
from torch.utils.data import DataLoader
import torchvision.datasets
from tensorboardX import SummaryWriter

writer=SummaryWriter("log")

dataset=torchvision.datasets.CIFAR10(root="./cifar10",train=False,transform=torchvision.transforms.ToTensor(),download=True)

dataloader=DataLoader(dataset,batch_size=64)

input=torch.tensor([[1,-0.5],
                    [-1,3]])

input=torch.reshape(input,(-1,1,2,2))

class Test(nn.Module):
    def __init__(self):
        super().__init__()
        self.relu1=ReLU()
    
    def forward(self,input):
        output=self.relu1(input)
        return output

a=0
t=Test()
for data in dataloader:
    imgs,targets=data
    writer.add_images("input",imgs,a)
    output=t(imgs)
    writer.add_images("output",output,a)
    a+=1
    if a>100:
        break

writer.close()