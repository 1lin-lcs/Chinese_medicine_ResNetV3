﻿from pickletools import optimize
import torchvision
from MobileNetV3 import *
from torch.utils.data import DataLoader
from tensorboardX import SummaryWriter
from torchvision import transforms,datasets
import os
import json

#train_data=torchvision.datasets.CIFAR10(root="./cifar10",train=True,transform=torchvision.transforms.ToTensor(),download=True)

#test_data=torchvision.datasets.CIFAR10(root="./cifar10",train=False,transform=torchvision.transforms.ToTensor(),download=True)

#normMean = [0.435315, 0.3777632, 0.28888336]
#normStd = [0.29441974, 0.24422123, 0.26730734]

data_transforms={'train':transforms.Compose(
                    [
                        transforms.RandomRotation(45),
                        transforms.RandomHorizontalFlip(p=0.5),
                        transforms.RandomVerticalFlip(p=0.5),
                        transforms.ColorJitter(brightness=0.2,contrast=0.1,saturation=0.1,hue=0.1),
                        transforms.RandomGrayscale(p=0.025),
                        transforms.ToTensor(),
                        transforms.Normalize([0.435, 0.377, 0.288],[0.294, 0.244, 0.267])
                        ]
                    ),
                 'validation':transforms.Compose(
                     [
                         transforms.Resize(256),
                         transforms.CenterCrop(224),
                         transforms.ToTensor(),
                         transforms.Normalize([0.435, 0.377, 0.288],[0.294, 0.244, 0.267])
                         ]
                    ),
                 'test':transforms.Compose(
                     [transforms.ToTensor()]
                    )
                     }


batch_size=32

data_dir=r"E:\文件\Documents\Python\pytorch_learning\Flowers"
train_dir=data_dir+"\train"
validation_dir=data_dir+"\validation"
test_dir=data_dir+"\test"

image_datasets={x:datasets.ImageFolder(os.path.join(data_dir,x),data_transforms[x]) for x in ['train','validation','test']}
dataloaders={x:DataLoader(image_datasets[x],batch_size=batch_size,shuffle=True) for x in ['train','validation','test']}
dataset_sizes={x:len(image_datasets[x]) for x in ['train','validation','test']}

with open(r"E:\文件\Documents\Python\pytorch_learning\Flower102_name.json",'r') as f:
    flower_name=json.load(f)


#训练的设备
device=torch.device("cuda" if torch.cuda.is_available() else "cpu")

#数据集的长度
train_data_size=len(image_datasets['train'])
validation_data_size=len(image_datasets['validation'])
test_data_size=len(image_datasets['test'])

#print("train_data_size: "+str(train_data_size)+"test_data_size: "+str(test_data_size))

#train_dataset=DataLoader(train_data,batch_size)

#test_dataset=DataLoader(validation_data,batch_size)

test=MobileNetV3_large(102)     #设置分类的数目
#test=torch.load(r"E:\文件\Documents\Python\pytorch_learning\model\test_11.pth")
test=test.to(device)    #用GPU，这个可以不用赋值
#损失函数
loss_fn=nn.CrossEntropyLoss()
loss_fn=loss_fn.to(device)      #用GPU，这个可以不同赋值
#优化器
learning_rate=0.1
optim=torch.optim.SGD(test.parameters(),learning_rate)

#记录训练的次数
total_train_step=0
#测试的次数
total_test_step=0
#训练的轮数
epoch=50

writer=SummaryWriter("./log")

for i in range(epoch):
    print("----------第{}轮训练开始----------".format(i+1))
    #训练步骤
    #如果有Dropout Batchnorm一定要要调用xxx.train()[train] 和xxx.eval()[test]
    test.train()
    total_train_loss=0
    for data in dataloaders['train']:
        optim.zero_grad()
        imgs,targets=data
        imgs=imgs.to(device)    #用GPU
        targets=targets.to(device)
        output=test(imgs)
        loss=loss_fn(output,targets)
        loss.backward()
        optim.step()
        total_train_step+=1
        total_train_loss+=loss
        if total_train_step%100==0:
            print("训练次数{}，Loss:{}".format(total_train_step,loss.item()))
            writer.add_scalar("train_loss",loss.item(),total_train_step)
    print("整体训练集上的Loss:{}".format(total_train_loss))
    #测试步骤开始
    total_test_loss=0
    total_accuray=0
    test.eval()
    with torch.no_grad():
        for data in dataloaders['validation']:
        #for data in dataloaders['train']:
            imgs,targets=data
            imgs=imgs.to(device)    #用GPU
            targets=targets.to(device)
            output=test(imgs)
            loss=loss_fn(output,targets)
            total_test_loss+=loss
            accuray=(output.argmax(1)==targets).sum()
            total_accuray+=accuray
    print("整体测试集上的Loss:{}".format(total_test_loss))
    print("整体测试集上的Accuray:{}".format(total_accuray/validation_data_size))       #这里要改总数
    writer.add_scalar("test_loss",total_test_loss,total_test_step)
    writer.add_scalar("accuray",total_accuray/validation_data_size,total_test_step)     #这里要改总数
    total_test_step+=1

    torch.save(test,"./model/test_{}.pth".format(i))
    print("模型已保存")
writer.close()

