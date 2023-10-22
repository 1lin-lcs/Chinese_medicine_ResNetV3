#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket()
{

}

/*! @brief ��������socketdescript�Ĺ��캯��
*/
MyTcpSocket::MyTcpSocket(qintptr socketdesc){
    socketDesc=socketdesc;
    this->setSocketDescriptor(socketdesc);
}

/*! @brief �������������ܵ������Ƿ���һ��������Json�ļ��ĺ���
*/
void MyTcpSocket::GetJsonFile(){
    JsonData->append(this->readAll());
    QJsonDocument doc=QJsonDocument(QJsonDocument::fromJson(JsonData,&JsonError));
    if(doc.isNull()||JsonError.error!=QJsonParseError::NoError){
        return;
    }
    emit(SendJsonFile(doc,socketDesc));
    JsonData.clear();
}

/*! @brief ���Ƿ��������õ�
*/
void MyTcpSocket::SendData(QJsonDocument* doc, qintptr socketdesc){
    if(socketdesc==socketDesc){
        QByteArray array=doc->toJson();
        this->write(array);
        delete doc;
    }
}

/*! @brief ���desc������
 *  @return socketDesc socket������
*/
qintptr MyTcpSocket::GetDesc(){
    return socketDesc;
}
