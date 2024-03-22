import torch
from torch import nn
from torchsummary import summary
import torch.nn.functional as F

class AlexNet(nn.Module):
    def __init__(self):
        super().__init__()
        self.ReLU=nn.ReLU()
        self.conv1=nn.Conv2d(in_channels=1,out_channels=96,kernel_size=11,stride=4)
        self.maxpool1=nn.MaxPool2d(kernel_size=3,stride=2)
        self.conv2=nn.Conv2d(in_channels=96,out_channels=256,kernel_size=5,padding=2)
        self.maxpool2=nn.MaxPool2d(kernel_size=3,stride=2)
        self.conv3=nn.Conv2d(in_channels=256,out_channels=384,kernel_size=3,padding=1)
        self.conv4=nn.Conv2d(in_channels=384,out_channels=384,kernel_size=3,padding=1)
        self.conv5=nn.Conv2d(in_channels=384,out_channels=256,kernel_size=3,padding=1)
        self.maxpool3=nn.MaxPool2d(kernel_size=3,stride=2)
        self.flatten=nn.Flatten()
        self.linear1=nn.Linear(6*6*256,4096)
        self.linear2=nn.Linear(4096,4096)
        self.linear3=nn.Linear(4096,4096)
    
    def forward(self,x):
        out=self.ReLU(self.conv1(x))
        out=self.maxpool1(out)
        out=self.ReLU(self.conv2(out))
        out=self.maxpool2(out)
        out=self.ReLU(self.conv3(out))
        out=self.ReLU(self.conv4(out))
        out=self.ReLU(self.conv5(out))
        out=self.maxpool3(out)
        out=self.flatten(out)#2 2304
        out=self.ReLU(self.linear1(out))
        out=F.dropout(out,p=0.5)
        out=self.ReLU(self.linear2(out))
        out=F.dropout(out,p=0.5)
        out=self.linear3(out)
        return out

if __name__=="__main__":
    device="cuda" if torch.cuda.is_available() else "cpu"
    model=AlexNet().to(device)
    print(summary(model,(1,227,227)))