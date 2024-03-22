import torch
from torch import nn
from torch.nn import MaxPool2d
from torch.utils.data import DataLoader
import torchvision
from tensorboardX import SummaryWriter

writer=SummaryWriter("log")

dataset=torchvision.datasets.CIFAR10(root="./cifar10",train=False,transform=torchvision.transforms.ToTensor(),download=True)
dataloader=DataLoader(dataset=dataset,batch_size=64)


input=torch.tensor([[1,2,0,3,1],
                    [0,1,2,3,1],
                    [1,2,1,0,0],
                    [5,2,3,1,1],
                    [2,1,0,1,1]],dtype=torch.float32)

input=torch.reshape(input,(-1,1,5,5))

class Test(nn.Module):
    def __init__(self):
        super().__init__()
        self.maxpool1=MaxPool2d(kernel_size=3,ceil_mode=True)
    def forward(self,input):
        output=self.maxpool1(input)
        return output

a=0
T=Test()
for data in dataloader:
    imgs,targets=data
    writer.add_images("input",imgs,a)
    output=T(imgs)
    writer.add_images("output",output,a)
    a+=1
    if a>100:
        break
writer.close()