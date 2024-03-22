from PIL import Image
import torchvision
from module import *

img_path="./Leaves/1001.jpg"
img=Image.open(img_path)

transform=torchvision.transforms.Compose([torchvision.transforms.Resize((32,32)),
                                          torchvision.transforms.ToTensor()
                                          ])

image=transform(img)
#print(image.shape)

model=torch.load("./model/test_9.pth")
#model=torch.load("./model/test_9.pth",map_location=torch.device("cpu"))
model=model.to("cuda")
#print(model)
image=torch.reshape(image,(1,3,32,32))
image=image.to("cuda")
model.eval()
with torch.no_grad():
    output=model(image)
print(output.argmax(1))