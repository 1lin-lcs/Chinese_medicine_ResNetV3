from PIL import Image
import os
import warnings

warnings.filterwarnings("error",category=UserWarning)
############################################
#mydir是DataSet数据集文件夹，需要自己设置       #
#mydir2是图片保存目录文件夹	，需要自己设置		   #
#这里生成的图片不会分类，配合Mean_std_cal.py使用#
############################################
mydir=r""
mydir2=r""
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
