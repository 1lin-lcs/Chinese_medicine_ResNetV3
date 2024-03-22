from tensorboardX import SummaryWriter
import torchvision
dataset_transform=torchvision.transforms.Compose([
        torchvision.transforms.ToTensor()
    ])

train_set=torchvision.datasets.CIFAR10(root="./cifar10",train=True,transform=dataset_transform,download=True)
test_set=torchvision.datasets.CIFAR10(root="./cifar10",train=False,transform=dataset_transform,download=True)

writer=SummaryWriter("log")
for i in range(10):
    img, target=test_set[i]
    writer.add_image("test_set",img,i)

writer.close()