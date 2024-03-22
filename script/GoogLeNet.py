'''
在GoogLeNet中,基本的卷积块被称为Inception块(Inception block)。
Inception块由四条并行路径组成。前三条路径使用窗口大小为1×1、3×3和5×5的卷积层,
从不同空间大小中提取信息。中间的两条路径在输入上执行1×1卷积,以减少通道数,
从而降低模型的复杂性。第四条路径使用3×3最大池化层,
然后使用1×1卷积层来改变通道数。
这四条路径都使用合适的填充来使输入与输出的高和宽一致,
最后我们将每条线路的输出在通道维度上连结,并构成Inception块的输出。
在Inception块中,通常调整的超参数是每层输出通道数。

1*1卷积的作用主要是:
实现跨通道的交互和信息整合
卷积核通道数的降维和升维,减少网络参数

全局平均池化层
优点
抑制过拟合:直接拉平做全连接层的方式依然保留了大量的空间
信息,假设feature map是32个通道的10*10图像,那么拉平就得到
了32*10*10的向量,如果是最后一层是对应两类标签,那么这一层
就需要3200*2的权重矩阵,而GAP不同,将空间上的信息直接用均
值代替,32个通道GAP之后得到的向量都是32的向量,那么最后一
层只需要32*2的权重矩阵。相比之下GAP网络参数会更少,而全连
接更容易在大量保留下来的空间信息上面过拟合。
GAP使特征图输入尺寸更加灵活:在前面举例里面可以看到特征
图经过GAP后的大小为1×1×C,这里的C为通道的数量,因此,此刻
神经网络参数不再与输入图像尺寸的大小有关,和输入特征图的通
道有关,也就是输入图像的长宽可以不固定。

缺点
信息丢失:全局平均池化会将每个通道内的特征信息压缩成一个
单一的数值。这可能会导致一些特征信息的丢失,特别是在深层网
络中,重要的细节可能会被平均掉,从而降低了网络的表达能力。
特征丰富性:在一些任务中,特征之间的关系和细节对于正确的
分类或预测是至关重要的。全局平均池化可能无法很好地捕捉这些
特征之间的复杂关系,从而影响了模型的性能。
梯度信息:在深度学习中,梯度信息是用于权重更新的关键。在
全局平均池化之后,特征图被降维为一个向量,导致梯度的传播变
得更加困难。这可能会导致梯度消失或爆炸,从而影响了优化过程。
复杂任务:对于一些复杂的任务,模型需要更多的层级和特征表
示,而全局平均池化可能无法提供足够的表达能力。
'''
import torch
from torch import nn
from torchsummary import summary

class Inception(nn.Module):
    def __init__(self,in_channels,out_channels1,out_channels2,out_channels3,out_channels4):
        super().__init__()
        self.ReLU=nn.ReLU()
        self.path1_1=nn.Conv2d(in_channels,out_channels1,kernel_size=1)
        self.path2_1=nn.Conv2d(in_channels,out_channels2[0],kernel_size=1)
        self.path2_2=nn.Conv2d(out_channels2[0],out_channels2[1],kernel_size=3,padding=1)
        self.path3_1=nn.Conv2d(in_channels,out_channels3[0],kernel_size=1)
        self.path3_2=nn.Conv2d(out_channels3[0],out_channels3[1],kernel_size=5,padding=2)
        self.path4_1=nn.MaxPool2d(kernel_size=3,padding=1,stride=1)
        self.path4_2=nn.Conv2d(in_channels,out_channels4,kernel_size=1)

    def forward(self,x):
        out1=self.ReLU(self.path1_1(x))
        out2=self.ReLU(self.path2_2(self.ReLU(self.path2_1(x))))
        out3=self.ReLU(self.path3_2(self.ReLU(self.path3_1(x))))
        out4=self.ReLU(self.path4_2(self.path4_1(x)))
        return torch.cat((out1,out2,out3,out4),dim=1)
        

class GoogLeNet(nn.Module):
    def __init__(self,Inception,in_channels,classnum):
        super().__init__()
        self.network1=nn.Sequential(
            nn.Conv2d(in_channels,out_channels=64,kernel_size=7,stride=2,padding=3),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=3,stride=2,padding=1),
            )
        self.network2=nn.Sequential(
            nn.Conv2d(in_channels=64,out_channels=64,kernel_size=1),
            nn.ReLU(),
            nn.Conv2d(in_channels=64,out_channels=192,kernel_size=3,padding=1),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=3,stride=2,padding=1)
            )
        self.network3=nn.Sequential(
            Inception(192,64,(96,128),(16,32),32),
            Inception(256,128,(128,192),(32,96),64),
            nn.MaxPool2d(kernel_size=3,stride=2,padding=1)
            )
        self.network4=nn.Sequential(
            Inception(480,192,(96,208),(16,48),64),
            Inception(512,160,(112,224),(24,64),64),
            Inception(512,128,(128,256),(24,64),64),
            Inception(512,112,(128,288),(32,64),64),
            Inception(528,256,(160,320),(32,128),128),
            nn.MaxPool2d(kernel_size=3,stride=2,padding=1)
            )
        self.network5=nn.Sequential(
            Inception(832,256,(160,320),(32,128),128),
            Inception(832,384,(192,384),(48,128),128),
            nn.AdaptiveAvgPool2d((1,1)),
            nn.Flatten(),
            nn.Linear(1024,classnum)
            )
        for m in self.modules():
            if isinstance(m,nn.Conv2d):
                nn.init.kaiming_normal_(m.weight,mode="fan_out",nonlinearity='relu')
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
        return out

if __name__=="__main__":
    device=torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model=GoogLeNet(Inception,1,10).to(device)
    print(summary(model,(1,224,224)))
