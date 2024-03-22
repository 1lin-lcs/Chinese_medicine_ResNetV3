import os

root_path=r"G:\中草药\malan(Flower)"

files=os.listdir(root_path)

for i in files:
    filename1=i.split("_")[0]
    #filename2=i.split("_")[1]
    id=i.split("_")[1]
    filename=filename1+"(Flower)_"+str(id)+".jpg"
    os.rename(os.path.join(root_path,i),os.path.join(root_path,filename))