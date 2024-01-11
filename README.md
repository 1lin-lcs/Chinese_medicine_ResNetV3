# Chinses_medicine_mobileNetV3

Training MobileNetV3 for Chinese Medicine

# Datasets:

https://pan.baidu.com/s/1TDAEwULt-rvOrU0z6-G9eQ 提取码: f3mh 

# Usage：

    训练文件(Training file)：train_mobileNetV3_Chinese_medicine.py

    测试模型文件(Test the model files)：test_model.py    single_test.py

    图片处理文件(Image processing files)：Resize_pic.py    split_Chinese_medicine_torch.py

    数据处理文件(Data processing file)：Mean_std_cal.py

    模型文件(Model file)：MobileNetV3.py

### 步骤：

    1、数据集在[https://pan.baidu.com/s/1TDAEwULt-rvOrU0z6-G9eQ](https://pan.baidu.com/s/1TDAEwULt-rvOrU0z6-G9eQ)中，使用split_Chinese_medicine_torch.py进行分类

    2、分类文件保存在DataSet文件夹中，之后可以使用train_mobileNetV3_Chinese_medicine.py进行训练

###### 如果使用自己的训练集：

        1、使用Resize_pic.py和Mean_std_cal.py计算出所需的平均值和标准差

        2、修改train_mobileNetV3_Chinese_medicine.py中的`transforms.Normalize`的内容

        3、剩下内容按照上面步骤的内容进行

2024：

---------------------1月11日更新--------------------

新增线程池，但不会启用

2023：

---------------------12月6日更新--------------------

更新了管理数据库用的软件

统一了qt的注释风格    

--------------------11月4日更新--------------------

更新了正式版的服务端，可以正常运行

--------------------10月7日更新--------------------

修复登录问题

--------------------10月4日更新--------------------

添加了测试用的服务端和客户端


