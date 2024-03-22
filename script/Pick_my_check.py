import os
import shutil

path_1=r"G:\train\commelina_communis_1"
name_1=os.path.basename(path_1)
path_2=r"G:\中草药\commelina_communis"
path_3=r"G:\中草药"
path_4=os.path.join(path_3,name_1)
if not os.path.exists(path_4):
    os.mkdir(path_4)

files=os.listdir(path_1)

for i in files:
    path=os.path.join(path_2,i)
    if os.path.exists(path):
        shutil.move(path,path_4)