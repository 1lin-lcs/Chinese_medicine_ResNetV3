from PIL import Image
from torchvision import transforms
from tensorboardX import SummaryWriter

img_path=r"E:\文件\Documents\Python\pytorch_learning\DataSet\train\ants_image\0013035.jpg"
img=Image.open(img_path)
#img.show()

writer=SummaryWriter("log")
ten_trans=transforms.ToTensor()
img_1=ten_trans(img)
writer.add_image("ToTensor",img_1)

trans_norm=transforms.Normalize([0.5,0.5,0.5],[0.5,0.5,0.5])
img_norm=trans_norm(img_1)
writer.add_image("To_tensor",img_norm,2)

print(img.size)
trans_resize=transforms.Resize((512,512))
img_resize=trans_resize(img)
img_resize=ten_trans(img_resize)
writer.add_image("Resize",img_resize,1)

trans_resize_2=transforms.Resize(512)
trans_compose=transforms.Compose([trans_resize_2,ten_trans])
img_resize_2=trans_compose(img)
writer.add_image("Resize",img_resize_2,2)

trans_crop=transforms.RandomCrop(512)
trans_compose_2=transforms.Compose([trans_crop,ten_trans])
for i in range(10):
    img_crop=trans_compose_2(img)
    writer.add_image("crop",img_crop,i)

writer.close()