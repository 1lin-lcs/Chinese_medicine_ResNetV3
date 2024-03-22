from MobileNetV3 import *
from ResNet import *
from VGG_NET16 import *
from torch.utils.data import DataLoader
from tensorboardX import SummaryWriter
from torchvision import transforms,datasets
import os

#train_data=torchvision.datasets.CIFAR10(root="./cifar10",train=True,transform=torchvision.transforms.ToTensor(),download=True)

#test_data=torchvision.datasets.CIFAR10(root="./cifar10",train=False,transform=torchvision.transforms.ToTensor(),download=True)
'''
#old
#normMean = [0.4359504, 0.49714467, 0.32818076]
#normStd = [0.2648534, 0.25331333, 0.27614158]
#new_1
#normMean = [0.43642107, 0.4951864, 0.32764333]
#normStd = [0.26524013, 0.25323054, 0.27601802]
#new_2
#normMean = [0.43831497, 0.48849636, 0.3261147]
#normStd = [0.26517445, 0.25026542, 0.27596965]
#new_3
#normMean = [0.44096467, 0.49001244, 0.32848793]
#normStd = [0.2664383, 0.251106, 0.27798843]
#new_4
#normMean = [0.4451392, 0.49365562, 0.33406493]
#normStd = [0.26767176, 0.25161767, 0.28121993]
#new_5
#normMean = [0.44707757, 0.4966539, 0.3295895]
#normStd = [0.26773494, 0.25163567, 0.28005472]
#new_6
#normMean = [0.4489284, 0.49835017, 0.33027962]
#normStd = [0.267985, 0.25168788, 0.27961758]
#new_7
#normMean = [0.44809732, 0.49743512, 0.32951194]
#normStd = [0.2678541, 0.25158697, 0.279476]
#new_8
#normMean = [0.44857118, 0.49775362, 0.33102104]
#normStd = [0.26790655, 0.2512692, 0.28066444]
#new_9
#normMean = [0.44851533, 0.49718806, 0.33081618]
#normStd = [0.26867545, 0.25197837, 0.28146]
#new_10
#normMean = [0.44867143, 0.49715912, 0.33097053]
#normStd = [0.268851, 0.25210798, 0.28164926]
#new_11
normMean = [0.44904798, 0.49718964, 0.33104357]
normStd = [0.2690128, 0.2523262, 0.28188667]
'''

normMean = [0.44904798, 0.49718964, 0.33104357]
normStd = [0.2690128, 0.2523262, 0.28188667]


data_transforms={'train':transforms.Compose(
                    [
                        transforms.RandomRotation(45),
                        transforms.RandomHorizontalFlip(p=0.5),
                        transforms.RandomVerticalFlip(p=0.5),
                        transforms.ColorJitter(brightness=0.2,contrast=0.1,saturation=0.1,hue=0.1),
                        transforms.RandomGrayscale(p=0.025),
                        transforms.ToTensor(),
                        transforms.Normalize(normMean,normStd)
                        ]
                    ),
                 'valid':transforms.Compose(
                     [
                         transforms.RandomRotation(45),
						 transforms.RandomHorizontalFlip(p=0.5),
                         transforms.RandomVerticalFlip(p=0.5),
                         transforms.ColorJitter(brightness=0.2,contrast=0.1,saturation=0.1,hue=0.1),
                         transforms.RandomGrayscale(p=0.025),
                         transforms.ToTensor(),
                         transforms.Normalize(normMean,normStd)
                         ]
                    ),
                 'test':transforms.Compose(
                     [
						transforms.RandomRotation(45),
                        transforms.RandomHorizontalFlip(p=0.5),
                        transforms.RandomVerticalFlip(p=0.5),
                        transforms.ColorJitter(brightness=0.2,contrast=0.1,saturation=0.1,hue=0.1),
                        transforms.RandomGrayscale(p=0.025),
                        transforms.ToTensor(),
                        transforms.Normalize(normMean,normStd)
						]
                    )
                     }


batch_size=64
f=open("./output.txt","w")
data_dir=r"/hy-tmp/CM_Ndata"
#data_dir=r"/hy-tmp/CM_Rdata"
train_dir=data_dir+"/train"
validation_dir=data_dir+"/valid"
test_dir=data_dir+"/test"

image_datasets={x:datasets.ImageFolder(os.path.join(data_dir,x),data_transforms[x]) for x in ['train','valid','test']}
dataloaders={x:DataLoader(image_datasets[x],batch_size=batch_size,shuffle=True) for x in ['train','valid','test']}
dataset_sizes={x:len(image_datasets[x]) for x in ['train','valid','test']}

#print(image_datasets['train'].class_to_idx)

#训练的设备
device=torch.device("cuda" if torch.cuda.is_available() else "cpu")

#数据集的长度
train_data_size=dataset_sizes['train']
validation_data_size=dataset_sizes['valid']
test_data_size=dataset_sizes['test']

#print("train_data_size: "+str(train_data_size)+"test_data_size: "+str(test_data_size))

#train_dataset=DataLoader(train_data,batch_size)

#test_dataset=DataLoader(validation_data,batch_size)

#test=MobileNetV3_small(79)     #设置分类的数目
test=ResNet34(59)
#test=VGG_Net16(54)
#test=torch.load(r"/hy-tmp/HModel/ACC_18_813.pth")
test=test.to(device)    #用GPU，这个可以不用赋值
#损失函数
loss_fn=nn.CrossEntropyLoss()
loss_fn=loss_fn.to(device)      #用GPU，这个可以不同赋值
#优化器
learning_rate=0.01
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
    f.write("----------第{}轮训练开始----------\n".format(i+1))
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
            f.write("训练次数{}，Loss:{}\n".format(total_train_step,loss.item()))
            writer.add_scalar("train_loss",loss.item(),total_train_step)
    print("整体训练集上的Loss:{}".format(total_train_loss))
    #测试步骤开始
    total_test_loss=0
    total_accuray=0
    test.eval()
    with torch.no_grad():
        for data in dataloaders['valid']:
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
    f.write("整体测试集上的Loss:{}\n".format(total_test_loss))
    print("整体测试集上的Accuray:{}".format(total_accuray/validation_data_size))       #这里要改总数
    f.write("整体测试集上的Accuray:{}\n".format(total_accuray/validation_data_size))
    writer.add_scalar("test_loss",total_test_loss,total_test_step)
    writer.add_scalar("accuray",total_accuray/validation_data_size,total_test_step)     #这里要改总数
    total_test_step+=1

    torch.save(test,"./model/test_{}.pth".format(i))
    print("模型已保存")
    f.write("模型已保存")
writer.close()
f.close()
