import torch
from torch import nn
from torch.nn import Linear
from tensorboardX import SummaryWriter
import torchvision
from torch.utils.data import DataLoader

dataset=torchvision.datasets.CIFAR10(root="./cifar10",train=False,transform=torchvision.transforms.ToTensor(),download=True)

dataloader=DataLoader(dataset,batch_size=64)

class Test(nn.Module):
    def __init__(self):
        super().__init__()
        self.linear1=Linear(32,10)
    def forward(self,input):
        output=self.linear1(input)
        return output

test=Test()
a=0

for data in dataloader:
    imgs,targets=data
    print(imgs.shape)
    output=test(imgs)
    print(output.shape)
    a+=1
    if a>100:
        break