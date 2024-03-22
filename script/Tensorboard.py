from tensorboardX import SummaryWriter
import numpy as np
from PIL import Image
image_path=r"E:\文件\Documents\Python\pytorch_learning\DataSet\train\ants_image\0013035.jpg"
image=Image.open(image_path)
image_array=np.array(image)
writer=SummaryWriter("log")
writer.add_image("test",image_array,2,dataformats="HW3")
for i in range(100):
    writer.add_scalar("y=x",i,i)
writer.close()