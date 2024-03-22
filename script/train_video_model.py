import torch
from torchvision.datasets import FashionMNIST
from torchvision import transforms
import torch.utils.data as Dataset
import numpy as np
from LeNet import *
from VGG_NET16 import *
from torch import nn
import pandas as pd
import copy
import time
from torchvision import ImageFolder

def train_val_data_process():
    ROOT_TRAIN=r''
    normalize=transforms.Normalize([],[])#mean,std
    train_transform=transforms.Compose(
        [
            transforms.Resize((224,224)),
            transforms.ToTensor(),
            normalize
        ]
    )
    #根据路径分配标签
    train_data=ImageFolder(ROOT_TRAIN,train_transform)

    train_data,val_data=Dataset.random_split(train_data,[round(0.8*len(train_data)),round(0.2*len(train_data))])
    train_dataloader=Dataset.DataLoader(dataset=train_data,
                                        batch_size=12,
                                        shuffle=True,
                                        num_workers=8)
    val_dataloader=Dataset.DataLoader(dataset=val_data,
                                        batch_size=12,
                                        shuffle=True,
                                        num_workers=8)
    return train_dataloader,val_dataloader

def train_data_process():
    train_data=FashionMNIST(root="./learn",
                           train=True,
                           transform=transforms.Compose(
                               [
                                   transforms.Resize(size=224),
                                    transforms.ToTensor()
                                ]
                               )
                           )
    train_data,val_data=Dataset.random_split(train_data,[round(0.8*len(train_data)),round(0.2*len(train_data))])
    train_dataloader=Dataset.DataLoader(dataset=train_data,
                                        batch_size=12,
                                        shuffle=True,
                                        num_workers=8)
    val_dataloader=Dataset.DataLoader(dataset=val_data,
                                        batch_size=12,
                                        shuffle=True,
                                        num_workers=8)
    return train_dataloader,val_dataloader

#train_val_data_process()

def train_model_process(model,train_dataloader,val_dataloader,num_epochs):
    device=torch.device("cuda" if torch.cuda.is_available() else "cpu")
    lr=0.001
    optimizer=torch.optim.Adam(model.parameters(),lr=lr)
    criterion=nn.CrossEntropyLoss()
    model=model.to(device)
    best_model_wts=copy.deepcopy(model.state_dict())#保存模型的参数
    best_acc=0.0
    #损失值列表
    train_loss_all=[]
    #准确度列表
    train_acc_all=[]
    val_loss_all=[]
    val_acc_all=[]
    #当前时间
    since=time.time()
    for epoch in range(0,num_epochs):
        print("Epoch {}/{}".format(epoch,num_epochs-1))
        print("-"*10)
        train_loss=0.0
        train_acc=0.0
        train_num=0
        val_num=0
        val_loss=0
        val_acc=0
        for step,(b_x,b_y) in enumerate(train_dataloader):
            b_x=b_x.to(device)
            b_y=b_y.to(device)
            model.train()
            output=model(b_x)
            #b_x batch_size*in_channels
            pre_lab=torch.argmax(output,dim=1)
            loss=criterion(output,b_y)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
            train_loss+=loss.item()*b_x.size(0)
            train_acc+=torch.sum(pre_lab==b_y.data)
            train_num+=b_x.size(0)
        for step,(b_x,b_y) in enumerate(val_dataloader):
            b_x=b_x.to(device)
            b_y=b_y.to(device)
            model.eval()
            output=model(b_x)
            pre_lab=torch.argmax(output,dim=1)
            loss=criterion(output,b_y)
            val_loss+=b_x.size(0)
            val_acc+=torch.sum(pre_lab==b_y.data)
            val_num+=b_x.size(0)
        train_acc_all.append(train_acc/train_num)
        train_loss_all.append(train_loss/train_num)
        val_acc_all.append(val_acc/val_num)
        val_loss_all.append(val_loss/val_num)
        print("{} train loss:{:4f} train acc:{:4f}".format(epoch,train_loss_all[-1],train_acc_all[-1]))
        print("{} val loss:{:4f} val acc:{:4f}".format(epoch,val_loss_all[-1],val_acc_all[-1]))
        if val_acc_all[-1]>best_acc:
            best_acc=val_acc_all[-1]
            best_model_wts=copy.deepcopy(model.state_dict())
        time_use=time.time()-since
        print("训练和验证耗费的时间：{:.0f}m{:.0f}s".format(time_use//60,time_use%60))
    model.load_state_dict(best_model_wts)
    torch.save(model.load_state_dict(best_model_wts),"./learn/best_model.pth")
    train_process=pd.DataFrame(data={"epoch":range(num_epochs),
                                     "train_loss_all":train_loss_all,
                                     "train_ass_all":train_acc_all,
                                     "val_loss_all":val_loss_all,
                                     "val_acc_all":val_acc_all
                                    })
    return train_process

def matploy_acc_loss(train_process):
    plt.figure(figsize=(12,4))
    plt.subplot(1,2,1)#一行两列
    plt.plot(train_process["epoch"],train_process.train_loss_all,"ro-",label="train loss")
    plt.plot(train_process["epoch"],train_process.val_loss_all,"ro-",label="val loss")
    plt.legend()
    plt.xlabel("epoch")
    plt.ylabel("loss")
    plt.plot(train_process["epoch"],train_process.train_acc_all,"ro-",label="train acc")
    plt.plot(train_process["epoch"],train_process.val_acc_all,"ro-",label="val acc")
    plt.legend()
    plt.xlabel("epoch")
    plt.ylabel("acc")
    plt.show()

if __name__=="__main__":
    model=VGG_Net16(1,10)
    train_dataloader,val_dataloader=train_data_process()
    train_process=train_model_process(model,train_dataloader,val_dataloader,20)
    matploy_acc_loss(train_process)
