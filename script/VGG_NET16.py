import torch
from torch import nn
from torchsummary import summary

device=torch.device("cuda" if torch.cuda.is_available() else "cpu")

#batch_size经验值在24以上

class VGG_Net16(nn.Module):
    def __init__(self,in_channels,classnum):
            super().__init__()
            self.network1=nn.Sequential(
                nn.Conv2d(in_channels,64,3,stride=1,padding=1),
                nn.BatchNorm2d(64),
                nn.ReLU(inplace=True),
                nn.Conv2d(64,64,3,stride=1,padding=1),
                nn.BatchNorm2d(64),
                nn.ReLU(inplace=True),
                nn.MaxPool2d(2,stride=2)
            )
            self.network2=nn.Sequential(
                  nn.Conv2d(64,128,3,stride=1,padding=1),
                  nn.BatchNorm2d(128),
                  nn.ReLU(inplace=True),
                  nn.Conv2d(128,128,3,stride=1,padding=1),
                  nn.BatchNorm2d(128),
                  nn.ReLU(inplace=True),
                  nn.MaxPool2d(2,stride=2)
            )
            self.network3=nn.Sequential(
                  nn.Conv2d(128,256,3,stride=1,padding=1),
                  nn.BatchNorm2d(256),
                  nn.ReLU(inplace=True),
                  nn.Conv2d(256,256,3,stride=1,padding=1),
                  nn.BatchNorm2d(256),
                  nn.ReLU(inplace=True),
                  nn.Conv2d(256,256,3,stride=1,padding=1),
                  nn.BatchNorm2d(256),
                  nn.ReLU(inplace=True),
                  nn.MaxPool2d(2,stride=2)
            )
            self.network4=nn.Sequential(
                  nn.Conv2d(256,512,3,stride=1,padding=1),
                  nn.BatchNorm2d(512),
                  nn.ReLU(inplace=True),
                  nn.Conv2d(512,512,3,stride=1,padding=1),
                  nn.BatchNorm2d(512),
                  nn.ReLU(inplace=True),
                  nn.Conv2d(512,512,3,stride=1,padding=1),
                  nn.BatchNorm2d(512),
                  nn.ReLU(inplace=True),
                  nn.MaxPool2d(2,stride=2)
            )
            self.network5=nn.Sequential(
                  nn.Conv2d(512,512,3,stride=1,padding=1),
                  nn.BatchNorm2d(512),
                  nn.ReLU(inplace=True),
                  nn.Conv2d(512,512,3,stride=1,padding=1),
                  nn.BatchNorm2d(512),
                  nn.ReLU(inplace=True),
                  nn.Conv2d(512,512,3,stride=1,padding=1),
                  nn.BatchNorm2d(512),
                  nn.ReLU(inplace=True),
                  nn.MaxPool2d(2,stride=2)
            )
            #这里的神经元个数可以自己更改
            self.network6=nn.Sequential(
                  nn.Flatten(),
                  nn.Linear(7*7*512,4096),
                  nn.ReLU(inplace=True),
                  nn.Dropout(0.5)
            )
            self.network7=nn.Sequential(
                  nn.Linear(4096,4096),
                  nn.ReLU(inplace=True),
                  nn.Dropout(0.5)
            )
            self.network8=nn.Sequential(
                  nn.Linear(4096,classnum),
                  #nn.Softmax()
            )
            #初始化权重
            for m in self.modules():
                if isinstance(m,nn.Conv2d):
                    nn.init.kaiming_normal_(m.weight,nonlinearity='relu')
                    if m.bias is not None:
                        nn.init.constant_(m.bias,0)
                elif isinstance(m,nn.Linear):
                    nn.init.normal_(m.weight,0,0.01)
                    if m.bias is not None:
                        nn.init.constant_(m.bias,0)
    
    def forward(self,x):
        out=self.network1(x)
        out=self.network2(out)
        out=self.network3(out)
        out=self.network4(out)
        out=self.network5(out)
        out=self.network6(out)
        out=self.network7(out)
        out=self.network8(out)
        return out

'''
1、vgg-block内的卷积层都是同结构的意味着输入和输出的尺寸一样,且卷积层可以堆叠复用,其中的实现是通过统一的size为3×3的kernel size+stride1+padding1实现。
2、maxpool层将前一层(vgg-block层)的特征缩减一半 使得尺寸缩减的很规整,从224-112-56-28-14-7。其中是通过pool size2+stride2实现。
3、深度较深,参数量够大 较深的网络层数使得训练得到的模型分类效果优秀,但是较大的参数对训练和模型保存提出了更大的资源要求。
4、较小的filter size/kernel size ** 这里全局的kernel size都为3×3,相比以前的网络模型来说,尺寸足够小。
'''
    

if __name__=="__main__":
    net=VGG_Net16(3,54).to(device)
    #x=torch.randn(1,3,224,224).to(device)
    #y=net(x)
    print(summary(net,(3,224,224)))
