#include "IdentityThread.h"

IdentityThread::IdentityThread(QString filename,QString path,QTcpSocket* socket)
{
	Filename = filename;
    DirPath = path;
    Tmpsocket = socket;
}

IdentityThread::~IdentityThread()
{}

void IdentityThread::run(){
    int nHold = PyGILState_Check();
    PyGILState_STATE gstate;
    if (!nHold) {
        gstate = PyGILState_Ensure();
    }

    Py_BEGIN_ALLOW_THREADS;
    Py_BLOCK_THREADS;

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyObject* pModule = PyImport_ImportModule("IdentityImg");//创建模块指针
    //emit(SendError("开始识别"));
    if (!pModule) {
        emit(SendError("获取模块指针出错"));
        return;
    }

    PyObject* pFunc = PyObject_GetAttrString(pModule, "testimg");//创建函数指针

    if (!pFunc) {
        emit(SendError("获取函数指针出错"));
        return;
    }

    PyObject* pPara = PyTuple_New(2);

    char* ImgF = nullptr;
    char* Path = nullptr;

    ImgF = QStringToChar(Filename);
    Path = QStringToChar(DirPath);

    PyTuple_SetItem(pPara, 0, Py_BuildValue("s", ImgF));
    PyTuple_SetItem(pPara, 1, Py_BuildValue("s", Path));

    PyObject* FuncBack = PyObject_CallObject(pFunc, pPara);

    char* output = NULL;

    PyArg_Parse(FuncBack, "s", &output);

    QString output1(output);
    emit(SendEnd(output1, Tmpsocket));

    Py_UNBLOCK_THREADS;
    Py_END_ALLOW_THREADS;

    if (!nHold) {
        PyGILState_Release(gstate);
    }
}

char* IdentityThread::QStringToChar(QString text) {
    return strdup(text.toStdString().c_str());
}