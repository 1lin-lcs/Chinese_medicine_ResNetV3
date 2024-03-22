from PIL import Image
import os
import warnings

warnings.filterwarnings("error",category=UserWarning)

mydir=r"E:\文件\Documents\Python\pytorch_learning\中草药"
mydir2=r"E:\test"
if not os.path.exists(mydir2):
    os.mkdir(mydir2)
os.chdir(mydir)
a=os.listdir()
for i in a:
    path=os.path.join(mydir,i)
    image_paths=os.listdir(path)
    for i in image_paths:
        try:
            img=Image.open(path+"\\"+i)
            if img.mode=="P":
                img=img.convert("RGB")
            out=img.resize((224,224),Image.Resampling.LANCZOS)
            out.save(mydir2+"\\"+i)
        except:
            print(path+"\\"+i)
