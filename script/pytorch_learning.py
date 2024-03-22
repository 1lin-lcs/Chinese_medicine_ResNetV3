from torch.utils.data import Dataset    #打开数据集
from PIL import Image
import os

class MyData(Dataset):
    def __init__(self,root_dir,label_dir):
        self.root_dir=root_dir
        self.label_dir=label_dir
        self.path=os.path.join(self.root_dir,self.label_dir)
        self.img_path=os.listdir(self.path)

    def __getitem__(self, index):
        img_name=self.img_path[index]
        img_item_path=os.path.join(self.root_dir,self.label_dir,img_name)
        img=Image.open(img_item_path)
        label=self.label_dir
        return img,label

    def __len__(self):
        return len(self.img_path)

root_dir=r"E:\文件\Documents\Python\pytorch_learning\hymenoptera_data\train"
ants_label_dir="ants"
bees_label_dir="bees"
if __name__=="__main__":
    a=MyData(root_dir,ants_label_dir)
    b=MyData(root_dir,bees_label_dir)
    trans_data=a+b  #拼接
    a[0][0].show()