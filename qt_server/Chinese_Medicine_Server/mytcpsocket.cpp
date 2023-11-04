#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket()
{}

/*! @brief 这是设置socketdescript的构造函数
*/
MyTcpSocket::MyTcpSocket(qintptr socketdesc){
    socketDesc=socketdesc;
    this->setSocketDescriptor(socketdesc);
}

/*! @brief 这是析构函数，用来释放数据的
*/
MyTcpSocket::~MyTcpSocket(){}

/*! @brief 这是用来处理受到数据是否是一个完整的Json文件的函数
*/
void MyTcpSocket::GetJsonFile(){
    JsonData.append(this->readAll());
    QJsonDocument* doc=new QJsonDocument(QJsonDocument::fromJson(JsonData,&JsonError));
    if(doc->isNull()||JsonError.error!=QJsonParseError::NoError){
        delete doc;
        return;
    }
    emit SendJsonFile(doc,socketDesc);
    JsonData.clear();
}

/*! @brief 这是发送数据用的
*/
void MyTcpSocket::SendData(QJsonDocument* doc, qintptr socketdesc){
    if(socketdesc==socketDesc){
        QByteArray array=doc->toJson();
        this->write(array);
        delete doc;
    }
}

/*! @brief 获得desc描述符
 *  @return socketDesc socket描述符
*/
qintptr MyTcpSocket::GetDesc(){
    return socketDesc;
}
