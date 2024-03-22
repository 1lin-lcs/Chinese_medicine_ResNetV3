import torch
from torch import nn

class Module(nn.Module):
    def __init__(self):
        super().__init__()
    
    def forward(self,input):
        output=input+1
        return output

Test=Module()
x=torch.tensor(1.0)
output=Test(x)
print(output)