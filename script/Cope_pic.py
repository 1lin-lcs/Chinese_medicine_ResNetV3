import os
from PIL import Image
import random

def get_files(path):
    if not os.path.isdir(path):
        raise NameError("不是文件夹")
        return
    files=os.listdir(path)
    return path,files
    

def rotate_pic(path,files):
    rotate_enum=[Image.Transpose.ROTATE_90,Image.Transpose.ROTATE_180,Image.Transpose.ROTATE_270]
    os.chdir(path)
    for file in files:
        file_name=file.split("_")
        file_suffix=file_name[-1].split(".")[1]
        file_num=int(file_name[-1].split(".")[0])+len(files)-1
        image=Image.open(file)
        rand=random.randint(0,2)
        image=image.transpose(rotate_enum[rand])
        #file=open(file_name[0]+"_"+str(file_num)+"."+file_suffix)
        try:
            image.save(file_name[0]+"_"+str(file_num)+"."+file_suffix)
        except OSError:
            image=image.convert('RGB')
            image.save(file_name[0]+"_"+str(file_num)+"."+file_suffix)
        #file.close()
        
if __name__=="__main__":
    #file_list=["Agastacherugosa","ChenopodiumAlbum","Gardenia","Hairyveinagrimony","jicai","juaner","MonochoriaVaginalis(yashecao)","qigucao","Rabdosiaserra","xunma"]
    #file_list=["boheye","denglongcao","kucai","malan","mangnoliaofficinalis","perillas","shuiqincai","tianhukui","Wormwood","zhajiangcao"]
    #file_list=["sedumF_sarmentosum","sedum_sarmentosum"]
    #file_list=["lotusseedF","lotusseed"]
    #file_list=["plantains"]
    #file_list=["EichhorniaCrassipes(fengyanlan)","EichhorniaCrassipes(fengyanlan)F"]
    file_list=["feipeng"]
    path=r"E:\文件\Documents\Python\pytorch_learning\中草药"
    for category in file_list:
        print("当前处理："+category)
        file_path,files=get_files(os.path.join(path,category))
        rotate_pic(file_path,files)