import numpy as np
import cv2
import os
from glob import glob
from tqdm import tqdm

# img_h, img_w = 32, 32
img_h, img_w = 224, 224  # 根据自己数据集适当调整，影响不大
means, stdevs = [], []
img_list = []
#########################################################################
#TRAIN_DATASET_PATH是全部图片的文件夹的路径，配合Resize_pic.py使用，需要自己设置#
#########################################################################
TRAIN_DATASET_PATH = r""

image_fns=[]

path=os.listdir(TRAIN_DATASET_PATH)
'''
for p in path:
    image_folder=os.path.join(TRAIN_DATASET_PATH,p)
    image_fns+=glob(image_folder+"\\*.jpg")
'''
image_fns = glob(TRAIN_DATASET_PATH+"\\*.jpg")

for single_img_path in tqdm(image_fns):
    img = cv2.imread(single_img_path)
    img = cv2.resize(img, (img_w, img_h))
    img = img[:, :, :, np.newaxis]
    img_list.append(img)


imgs = np.concatenate(img_list, axis=3)
imgs = imgs.astype(np.float32) / 255.

for i in range(3):
    pixels = imgs[:, :, i, :].ravel()  # 拉成一行
    means.append(np.mean(pixels))
    stdevs.append(np.std(pixels))

# BGR --> RGB ， CV读取的需要转换，PIL读取的不用转换
means.reverse()
stdevs.reverse()

print("normMean = {}".format(means))
print("normStd = {}".format(stdevs))