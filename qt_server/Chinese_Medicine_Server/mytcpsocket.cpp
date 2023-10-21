#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket()
{

}

/*! @brief 这是用来处理受到数据是否是一个完整的Json文件的函数
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
