#include "mytcptask.h"
#include "mytcpsocket.h"

MyTcpTask::MyTcpTask(){}

MyTcpTask::~MyTcpTask(){}

/*!
 * \brief 获得接收的Json内容
 * \param doc 接收的Json文档
 * \param handle socket套接字
 */
void MyTcpTask::GetJsonFile(QJsonDocument* doc,qintptr handle){
    emit SendJsonFile(doc,handle);
}

/*!
 * \brief 获得要发送的Json内容
 * \param doc 发送的Json文档
 * \param handle socket套接字
 */
void MyTcpTask::GetData(QJsonDocument* doc,qintptr handle){
    emit SendData(doc,handle);
}

/*!
 * \brief 设置套接字
 * \param handle socket套接字
 */
void MyTcpTask::setSocketDescriptor(qintptr handle){
    socketDescriptor=handle;
}

/*!
 * \brief 重载run函数，实现tcpsocket多线程
 */
void MyTcpTask::run(){
    MyTcpSocket* socket=new MyTcpSocket(socketDescriptor);
    connect(socket,&MyTcpSocket::SendJsonFile,this,&MyTcpTask::GetJsonFile);
    connect(socket,&MyTcpSocket::readyRead,socket,&MyTcpSocket::GetJsonFile);
    connect(this,&MyTcpTask::SendData,socket,&MyTcpSocket::SendData);
    connect(socket,&MyTcpSocket::disconnected,socket,&MyTcpSocket::deleteLater);
}
