#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket()
{}

/*! \brief 这是设置socketdescript的构造函数
*/
MyTcpSocket::MyTcpSocket(qintptr socketdesc){
    socketDesc=socketdesc;
    this->setSocketDescriptor(socketdesc);
}

/*! \brief 这是析构函数，用来释放数据的
*/
MyTcpSocket::~MyTcpSocket(){}

/*! \brief 这是用来处理受到数据是否是一个完整的Json文件的函数
*/
void MyTcpSocket::GetJsonFile(){
    waitForReadyRead(3000);
    JsonData.append(this->readAll());
    QJsonDocument* doc=new QJsonDocument(QJsonDocument::fromJson(JsonData,&JsonError));
    if(doc->isNull()||JsonError.error!=QJsonParseError::NoError){
        delete doc;
        return;
    }

#ifdef ShowInformation
    ShowData(doc);
#endif

    emit SendJsonFile(doc,socketDesc);
    JsonData.clear();
}

/*! \brief 这是发送数据用的
*/
void MyTcpSocket::SendData(QJsonDocument* doc, qintptr socketdesc){
    if(socketdesc==socketDesc){
        QByteArray array=doc->toJson();
        this->write(array);
        delete doc;
    }
}

/*! \brief 获得desc描述符
 *  \return socketDesc socket描述符
*/
qintptr MyTcpSocket::GetDesc(){
    return socketDesc;
}

#ifdef ShowInformation
/*! \brief 展示接收到的信息
 *  \param data 接收到的完整的Json内容
 */
void MyTcpSocket::ShowData(QJsonDocument* data){
    QJsonObject top=data->object();
    if(top.contains("status")){
        QJsonObject status=top.value("status").toObject();
        if(status.contains("server"))
            qInfo()<<"server:"<<status.value("server").toInt();
        if(status.contains("client"))
            qInfo()<<"client:"<<status.value("client").toInt();
        if(status.contains("error"))
            qInfo()<<"error:"<<status.value("error").toInt();
    }
    if(top.contains("response")){
        QJsonObject response=top.value("response").toObject();
        if(response.contains("data"))
            qInfo()<<"data:"<<response.value("data").toString();
        if(response.contains("external"))
            qInfo()<<"external:"<<response.value("external").toString();
    }
    if(top.contains("information")){
        QJsonObject information=top.value("information").toObject();
        if(information.contains("username"))
            qInfo()<<"username:"<<information.value("username").toString();
        if(information.contains("password"))
            qInfo()<<"password:"<<information.value("password").toString();
        if(information.contains("email"))
            qInfo()<<"email:"<<information.value("email").toString();
    }
}
#endif
