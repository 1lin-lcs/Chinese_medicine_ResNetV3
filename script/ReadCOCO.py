import torchvision
from PIL import ImageDraw

coco_dataset=torchvision.datasets.CocoDetection(root=r"E:\文件\Documents\Python\pytorch_learning\val2017",
                                                annFile=r"E:\文件\Documents\Python\pytorch_learning\val2017\annotations\instances_val2017.json")

print(coco_dataset[0])
image,info=coco_dataset[0]
image_handler=ImageDraw.ImageDraw(image)
for annotations in info:
    x_min,y_min,width,height=annotations['bbox']
    image_handler.rectangle(((x_min,y_min),(x_min+width,y_min+height)))
    print(x_min)
image.show()