import torch
from torch import nn
from torch.nn import *
from torch.utils.data import DataLoader
import tensorvision
from tensorboardX import SummaryWriter
import torchvision

dataset=torchvision.datasets.CIFAR10(root="./cifar10",train=False,transform=torchvision.transforms.ToTensor(),download=True)

dataloader=DataLoader(dataset,batch_size=64)

class Test(nn.Module):
    def __init__(self):
        super().__init__()
        '''
        self.conv1=Conv2d(3,32,5,padding=2)
        self.maxpool1=MaxPool2d(2)
        self.conv2=Conv2d(32,32,5,padding=2)
        self.maxpool2=MaxPool2d(2)
        self.conv3=Conv2d(32,64,5,padding=2)
        self.maxpool3=MaxPool2d(2)
        self.flatten=Flatten()
        self.linear1=Linear(1024,64)
        self.linear2=Linear(64,10)
        '''
        self.model1=Sequential(
                Conv2d(3,32,5,padding=2),
                MaxPool2d(2),
                Conv2d(32,32,5,padding=2),
                MaxPool2d(2),
                Conv2d(32,64,5,padding=2),
                MaxPool2d(2),
                Flatten(),
                Linear(1024,64),
                Linear(64,10)
            )
    
    def forward(self,input):
        '''
        x=self.conv1(input)
        x=self.maxpool1(x)
        x=self.conv2(x)
        x=self.maxpool2(x)
        x=self.conv3(x)
        x=self.maxpool3(x)
        x=self.flatten(x)
        x=self.linear1(x)
        x=self.linear2(x)
        '''
        x=self.model1(input)
        return x

test=Test()
'''
input=torch.ones((64,3,32,32))
output=test(input)
print(output.shape)
writer=SummaryWriter("log")
writer.add_graph(test,input)
writer.close()
'''
loss=CrossEntropyLoss()
optim=torch.optim.SGD(test.parameters(),lr=0.01)
for epoch in range(50):
    result=0.0
    for data in dataloader:
        optim.zero_grad()
        imgs,targets=data
        output=test(imgs)
        result=loss(output,targets)
        result.backward() 
        optim.step()    #参数更新
        result+=result
    print(result)
