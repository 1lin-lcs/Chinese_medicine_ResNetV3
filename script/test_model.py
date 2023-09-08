import torch
from MobileNetV3 import *
from torchvision import datasets,transforms
from torch.utils.data import DataLoader
from torch.nn import *
import os

############################################
#data_dir是数据集文件夹		               #
#model_path是model文件所在文件夹			   #
############################################

data_dir=r"../DataSet"
model_path=r"../model"
batch_size=32

trans=transforms.Compose([transforms.ToTensor(),transforms.Normalize([0.435, 0.497, 0.328],[0.264, 0.253, 0.276])])
image_data=datasets.ImageFolder(os.path.join(data_dir,"test"),trans)
#image_data=datasets.ImageFolder(os.path.join(data_dir,"valid"),trans)
testdata=DataLoader(image_data,batch_size=batch_size,shuffle=True)
testdata_size=len(image_data)

device=torch.device("cuda" if torch.cuda.is_available() else "cpu")

model_file=os.listdir(model_path)

loss_fn=CrossEntropyLoss()

for i in model_file:
    test_model=torch.load(os.path.join(model_path,i))
    test_model=test_model.to(device)
    accuary=0
    loss_sum=0

    test_model.eval()
    print("开始测试")
    print("测试集总数是{}".format(testdata_size))
    with torch.no_grad():
        for data in testdata:       #这里一次for是batch_size的大小数据
            imgs,targets=data
            imgs=imgs.to(device)
            targets=targets.to(device)
            output=test_model(imgs)
            loss=loss_fn(output,targets)
            loss_sum+=loss
            acc=(output.argmax(1)==targets).sum()
            accuary+=acc
    print(i)
    print("accuary is {}, loss_sum is {}".format(accuary/testdata_size,loss_sum))