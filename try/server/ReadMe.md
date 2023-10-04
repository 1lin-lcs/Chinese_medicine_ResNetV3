# 这是测试用的服务端

启动前先设置configuration

configuration字段的解释：

- IsAutoQuit：是否启用自动关闭的功能
- IntervalTime：服务器检测是否有连接的间隔时常，单位是秒
- QuitCount：没有连接的间隔次数
- information：数据库信息
  - HostName：数据库地址
  - Port：数据库端口
  - DataBaseName：数据库名字
  - Username：数据库用户
  - Password：数据库用户密码



使用pro文件在Qt Creator进行构建

需要连接的lib有network、sql和python

sql是使用MySQL

python所需的include已提供，lib可以用自己安装的

python安装pytorch、torchvision、PIL库



暂时多个客户端连接是有问题的


