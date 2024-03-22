from torchvision import transforms
from PIL import Image
from tensorboardX import SummaryWriter

img_path=r"E:\文件\Documents\Python\pytorch_learning\DataSet\train\ants_image\24335309_c5ea483bb8.jpg"
image=Image.open(img_path)

writer=SummaryWriter("log")

tensor_trnas=transforms.ToTensor()#这里不可以加参数
TenImg=tensor_trnas(image)

writer.add_image("test",TenImg)
writer.close()

print(TenImg)