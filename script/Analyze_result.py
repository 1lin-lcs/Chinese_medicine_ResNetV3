import torch
from torch.utils.data import DataLoader
from torchvision import datasets,transforms
from ResNet import *
import json

device=torch.device("cuda" if torch.cuda.is_available() else "cpu")

json_data_path=r"E:\文件\Documents\Python\pytorch_learning\Chinese_Medicine_N.json"
file=open(json_data_path,'r')
json_data=json.load(file)
file.close()

normMean = [0.44851533, 0.49718806, 0.33081618]
normStd = [0.26867545, 0.25197837, 0.28146]


def Load_data(Root_path):
    trans=transforms.Compose(
        [
            transforms.Resize((224,224)),
            transforms.RandomRotation(45),
            transforms.ToTensor(),
            transforms.Normalize(normMean,normStd)
        ])
    data_folder=datasets.ImageFolder(Root_path,trans)
    test_data=DataLoader(data_folder,batch_size=1,shuffle=True)
    return test_data,len(data_folder.classes)

def test_model(model,dataSet,classNum):
    category_acc=[0]*classNum
    category_num=[0]*classNum
    model.eval()
    with torch.no_grad():
        for _,(img,target) in enumerate(dataSet):
            img=img.to(device)
            target=target.to(device)
            output=model(img)
            pre_lab=torch.argmax(output,dim=1)
            if pre_lab.item()==target.item():           
                category_acc[target.item()]+=1
            category_num[target.item()]+=1
    ACC=[0]*classNum
    for i in range(0,len(category_num)):
        ACC[i]=float(category_acc[i]/category_num[i])
    return ACC
            
            
            
        
if __name__=="__main__":
    datasets,classNum=Load_data(r"E:\文件\Documents\Python\pytorch_learning\CM_Ndata\test")
    model=torch.load(r"E:\文件\Documents\Python\pytorch_learning\bak\test\ACC_18_921.pth").to(device)
    Acc=test_model(model,datasets,classNum)
    output_file=open("./AnalyzeData/Each_plant_acc_resnet18_N2.csv","w")
    for i in range(0,len(Acc)):
        output_file.write(json_data.get(str(i))+","+str(Acc[i])+'\n')
    output_file.close()