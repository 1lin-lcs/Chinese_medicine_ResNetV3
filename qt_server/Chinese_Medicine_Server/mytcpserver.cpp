#include "mytcpserver.h"

MyTcpServer::MyTcpServer()
{}

/*! @brief �������ԭ�����麯����Ϊ��ʵ�ֶ��̣߳��õ�socketDescriptor��Ҳ��Ϊhandle
 * @param handle ����socketDescriptor
 * @note �ٷ��ĵ� socketDescriptor argument is the native socket descriptor for the accepted connection
 */
void MyTcpServer::incomingConnection(qintptr handle){
    emit(SocketDesc(handle));
}
