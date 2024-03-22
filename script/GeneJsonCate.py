import os
import json
from torchvision import datasets

if __name__=="__main__":
    path=r"E:\文件\Documents\Python\pytorch_learning\CM_Ndata"
    ctype="train"
    fpath=r"E:\文件\Documents\Python\pytorch_learning"
    filename="Chinese_Medicine_N.json"
    dataset=datasets.ImageFolder(os.path.join(path,ctype))
    JsonFile={}
    for i in range(0,len(dataset.classes)):
        JsonFile.update({str(i):dataset.classes[i]})
    #print(JsonFile)
    j=json.dumps(JsonFile,indent=4, sort_keys=False, ensure_ascii=False)
    f=open(os.path.join(fpath,filename),'w')
    f.write(j)
    f.close()
    