#include "mytcpserver.h"

MyTcpServer::MyTcpServer()
{}

/*! @brief 这个覆盖原来的虚函数，为了实现多线程，得到socketDescriptor，也称为handle
 * @param handle 就是socketDescriptor
 * @note 官方文档 socketDescriptor argument is the native socket descriptor for the accepted connection
 */
void MyTcpServer::imcomingConnection(qintptr handle){
    emit SocketDesc(handle);
    emit newConnection();
}
