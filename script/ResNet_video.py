'''
在残差块这样的结构引入之前,如果一个神经网络模
型的深度太深,可能会带来梯度消失和梯度爆炸的问题
(如下图),随着一些正则化方法的应用可以缓解此问题
但是随着layer深度的继续加深,又带来了模型退化这样
的问题。

那么,这里就有一个问题,为什么加深网络会带来
退化问题?即使新增的这些layer什么都没学习,保持恒
等输出(所有的weight都设为1),那么按理说网络的精度
也应该等于原有未加深时的水平,如果新增的layer学习
到了有用特征,那么必然加深过后的模型精度会大于未
加深的原网络。看起来对于网络的精度加深后都应该大
于等于加深前才对啊?实际上,让新增的layer保持什么
都不学习的恒等状态,恰恰很困难,因为在训练过程中
每一层layer都通过线性修正单元relu处理,而这样的处
理方式必然带来特征的信息损失(不论是有效特征or冗
余特征)。所以上述假设的前提是不成立的,简单的堆叠
layer必然会带来退化问题。

ResNet的基础架构-残差块(residual block)。
在残差块中,输入可通过跨层数据线路更快地向
前传播。
ResNet沿用了VGG完整的3×3卷积层设计。残
差块里首先有2个有相同输出通道数的3×3卷积
层。每个卷积层后接一个批量规范化层和ReLU
激活函数。然后我们通过跨层数据通路,跳过这
2个卷积运算,将输入直接加在最后的ReLU激活
函数前。这样的设计要求2个卷积层的输出与输
入形状一样,从而使它们可以相加。如果想改变
通道数,就需要引入一个额外的1×1卷积层来将
输入变换成需要的形状后再做相加运算。

BN是由Google于2015年提出,这是一个深度神
经训练的技巧,它不仅可以加快了模型的收敛速
度,使训练深层网络模型更加容易和稳定。目前
BN已经成为几乎所有卷积神经网络的标配技巧了。
从字面意思看来Batch Normalization(简称BN)
就是对每一批数据进行归一化,确实如此,对于
训练中某一个batch的数据{x1,x2, ... ,xn},注意这个
数据是可以输入也可以是网络中间的某一层输出。
在BN出现之前,我们的归一化操作一般都在数据
输入层,对输入的数据进行求均值以及求方差做
归一化,但是BN的出现打破了这一个规定,我们
可以在网络中任意一层进行归一化处理,因为我
们现在所用的优化方法大多都是min-batch SGD,
所以我们的归一化操作就成为Batch Normalization。

BN步骤主要分为4步:
(1)求每一个训练批次数据的均值
(2)求每一个训练批次数据的方差
(3)使用求得的均值和方差对该批次的训练数据
做归一化,获得0-1分布。其中s是为了避免除数
为0时所使用的微小正数。
(4) 尺度变换和偏移:将xi乘以y调整数值大小,
再加上β增加偏移后得到yi,这里的y是尺度因子,
β是平移因子。这一步是BN的精髓,由于归一化后
的xi基本会被限制在正态分布下,使得网络的表达
能力下降。为解决该问题,我们引入两个新的参
数:y,β。y和β是在训练时网络自己学习得到的。

BN解决的问题

减均值除方差后,数据就被移到中心区域,
对于大多数激活函数而言,这个区域的梯度都是最
大的或者是有梯度的这可以看做是一种对抗梯度消
失的有效手段。如果对于每一层数据都那么做的话,
数据的分布总是在随着变化敏感的区域,相当于不
用考虑数据分布变化了,这样训练起来更有效率。

BN的本质就是利用优化变一下方差大小和
均值位置,使得新的分布更切合数据的真实分布,
保证模型的非线性表达能力。BN的极端的情况就是
这两个参数等于mini-batch的均值和方差,那么经
过batch normalization之后的数据和输入完全一样,
当然一般的情况是不同的。

BN总结：
(1) 加速收敛
(2)解决梯度消失和梯度爆炸
(3)可以不需要小心翼翼地设置权重初始化
初始化对学习的影响减小了,可以不那么
小心地设置初始权重。举例来说,对于一个单
元的输入值,不管权重w,还是放缩后的权重
kw,BN过后的值都是一样的,这个k被消掉了,
对于学习来说,激活值是一样的。

ResNet总结

残差网络的出现使人们摆脱深度的束缚,大幅改善了深度
神经网络中的模型退化问题,使网络层数从数十层跃升至几百上
千层,大幅提高了模型精度,通用性强适合各种类型的数据集和
任务。残差块和shortcut这种优秀的设计也极大影响了后面的网
络结构发展。
'''

import torch
from torch import nn
from torchsummary import summary

class Residual(nn.Module):
    def __init__(self,in_channels,out_channels,use_1conv=False,stride=1):
        super().__init__()
        self.ReLU=nn.ReLU()
        self.conv1=nn.Conv2d(in_channels=in_channels,out_channels=out_channels,kernel_size=3,stride=stride,padding=1)
        self.conv2=nn.Conv2d(in_channels=out_channels,out_channels=out_channels,kernel_size=3,padding=1)
        self.bn1=nn.BatchNorm2d(out_channels)
        self.bn2=nn.BatchNorm2d(out_channels)
        if use_1conv:
            self.conv3=nn.Conv2d(in_channels=in_channels,out_channels=out_channels,kernel_size=1,stride=stride)
        else:
            self.conv3=None
    def forward(self,x):
        out=self.ReLU(self.bn1(self.conv1(x)))
        out=self.bn2(self.conv2(out))
        if self.conv3:
            x=self.conv3(x)
        out=self.ReLU(x+out)
        return out
        
class ResNet18(nn.Module):
    def __init__(self,Residual,in_channels,classnum):
        super().__init__()
        self.network1=nn.Sequential(
                nn.Conv2d(in_channels=in_channels,out_channels=64,kernel_size=7,stride=2,padding=3),
                nn.BatchNorm2d(64),
                nn.ReLU(),
                nn.MaxPool2d(kernel_size=3,stride=2,padding=1)
            )
        self.network2=nn.Sequential(
                Residual(64,64),
                Residual(64,64)
            )
        self.network3=nn.Sequential(
                Residual(64,128,True,2),
                Residual(128,128)
            )                
        self.network4=nn.Sequential(
                Residual(128,256,True,2),
                Residual(256,256)
            )
        self.network5=nn.Sequential(
                Residual(256,512,True,2),
                Residual(512,512)
            )
        self.network6=nn.Sequential(
                nn.AdaptiveAvgPool2d((1,1)),
                nn.Flatten(),
                nn.Linear(512,classnum)
            )
    def forward(self,x):
        out=self.network1(x)
        out=self.network2(out)
        out=self.network3(out)
        out=self.network4(out)
        out=self.network5(out)
        out=self.network6(out)
        return out

if __name__=="__main__":
    device=torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model=ResNet18(Residual,1,10).to(device)
    print(summary(model,(1,224,224)))
