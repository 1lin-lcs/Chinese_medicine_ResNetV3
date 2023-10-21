#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket()
{

}

/*! @brief �������������ܵ������Ƿ���һ��������Json�ļ��ĺ���
 *
*/
void MyTcpSocket::GetJsonFile(){
    JsonData->append(this->readAll());
    QJsonDocument doc=QJsonDocument(QJsonDocument::fromJson(JsonData,&JsonError));
    if(doc.isNull()||JsonError.error!=QJsonParseError::NoError){
        return;
    }
    emit(SendJsonFile(doc));
    JsonData.clear();
}
