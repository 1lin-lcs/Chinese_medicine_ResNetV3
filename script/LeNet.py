import torch
from torch import nn
from torchsummary import summary

class LeNet(nn.Module):
    def __init__(self):
        super().__init__()
        self.c1=nn.Conv2d(in_channels=1,out_channels=6,kernel_size=5,padding=2)
        self.sig1=nn.Sigmoid()
        self.a1=nn.AvgPool2d(kernel_size=2,stride=2)
        self.c2=nn.Conv2d(in_channels=6,out_channels=16,kernel_size=5)
        self.a2=nn.AvgPool2d(kernel_size=2,stride=2)
        self.flatten=nn.Flatten()
        self.line1=nn.Linear(400,120)
        self.line2=nn.Linear(120,84)
        self.line3=nn.Linear(84,10)
    def forward(self,x):
        out=self.sig1(self.c1(x))
        out=self.a1(out)
        out=self.sig1(self.c2(out))
        out=self.a2(out)
        #print(out.size(0),out.size(1),out.size(2),out.size(3))
        out=self.flatten(out)
        out=self.line1(out)
        out=self.line2(out)
        out=self.line3(out)
        return out

if __name__=="__main__":
    device=torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model=LeNet().to(device)
    print(summary(model,(1,28,28)))
    

