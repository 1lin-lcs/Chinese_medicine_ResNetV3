from torch import nn
from torch.nn import*
import torch

class Test(nn.Module):
    def __init__(self):
        super().__init__()
        self.model=Sequential(
                Conv2d(3,32,5,1,2),
                MaxPool2d(2),
                Conv2d(32,32,5,1,2),
                MaxPool2d(2),
                Conv2d(32,64,5,1,2),
                MaxPool2d(2),
                Flatten(),
                Linear(1024,64),
                Linear(64,10)
            )

    def forward(self,input):
        output=self.model(input)
        return output


if __name__=="__main__":
    test=Test()
    input=torch.ones((64,3,32,32))
    output=test(input)
    print(output.shape)