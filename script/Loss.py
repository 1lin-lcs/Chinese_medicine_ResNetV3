import torch 
from torch.nn import *

input=torch.tensor([1,2,3],dtype=torch.float32)
output=torch.tensor([1,2,5],dtype=torch.float32)

input=torch.reshape(input,(1,1,1,3))
output=torch.reshape(output,(1,1,1,3))

l1loss=L1Loss()
#l1loss=L1Loss(reduction='sum')
result=l1loss(input,output)

mseloss=MSELoss()
result_mse=mseloss(input,output)

print(result)
print(result_mse)

x=torch.tensor([0.1,0.2,0.3])
y=torch.tensor([0,1,0],dtype=torch.float32)
loss_cross=CrossEntropyLoss()
result_cross=loss_cross(x,y)
print(result_cross)