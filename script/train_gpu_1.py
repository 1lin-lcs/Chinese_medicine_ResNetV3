from pickletools import optimize
import torchvision
from module import *
from torch.utils.data import DataLoader
from tensorboardX import SummaryWriter

train_data=torchvision.datasets.CIFAR10(root="./cifar10",train=True,transform=torchvision.transforms.ToTensor(),download=True)

test_data=torchvision.datasets.CIFAR10(root="./cifar10",train=False,transform=torchvision.transforms.ToTensor(),download=True)

#数据集的长度
train_data_size=len(train_data)
test_data_size=len(test_data)

#print("train_data_size: "+str(train_data_size)+"test_data_size: "+str(test_data_size))

train_dataset=DataLoader(train_data,batch_size=64)

test_dataset=DataLoader(test_data,batch_size=64)

test=Test()
if torch.cuda.is_available():
    test=test.cuda()    #用GPU
#损失函数
loss_fn=CrossEntropyLoss()
if torch.cuda.is_available():
    loss_fn=loss_fn.cuda()      #用GPU
#优化器
learning_rate=0.01
optim=torch.optim.SGD(test.parameters(),learning_rate)

#记录训练的次数
total_train_step=0
#测试的次数
total_test_step=0
#训练的轮数
epoch=10

writer=SummaryWriter("./log")

for i in range(epoch):
    print("----------第{}轮训练开始----------".format(i+1))
    #训练步骤
    #如果有Dropout Batchnorm一定要要调用xxx.train()[train] 和xxx.eval()[test]
    test.train()
    for data in train_dataset:
        optim.zero_grad()
        imgs,targets=data
        if torch.cuda.is_available():
            imgs=imgs.cuda()    #用GPU
            targets=targets.cuda()
        output=test(imgs)
        loss=loss_fn(output,targets)
        loss.backward()
        optim.step()
        total_train_step+=1
        if total_train_step%100==0:
            print("训练次数{}，Loss:{}".format(total_train_step,loss.item()))
            writer.add_scalar("train_loss",loss.item(),total_train_step)

    #测试步骤开始
    total_test_loss=0
    total_accuray=0
    test.eval()
    with torch.no_grad():
        for data in test_dataset:
            imgs,targets=data
            if torch.cuda.is_available():
                imgs=imgs.cuda()    #用GPU
                targets=targets.cuda()
            output=test(imgs)
            loss=loss_fn(output,targets)
            total_test_loss+=loss
            accuray=(output.argmax(1)==targets).sum()
            total_accuray+=accuray
    print("整体测试集上的Loss:{}".format(total_test_loss))
    print("整体测试集上的Accuray:{}".format(total_accuray/test_data_size))
    writer.add_scalar("test_loss",total_test_loss,total_test_step)
    writer.add_scalar("accuray",total_accuray,total_test_step)
    total_test_step+=1

    torch.save(test,"./model/test_{}.pth".format(i))
    print("模型已保存")
writer.close()
