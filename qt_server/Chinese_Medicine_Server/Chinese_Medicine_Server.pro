QT = core

QT+=network
QT+=sql

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        chinese_medicine_server.cpp \
        main.cpp \
        mydatabase.cpp \
        mytcpserver.cpp \
        mytcpsocket.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    chinese_medicine_server.h \
    mydatabase.h \
    mytcpserver.h \
    mytcpsocket.h

#DEFINES+=UsePython
DEFINES+=UseC++

# 使用Python

contains(DEFINES,UsePython){
    LIBS+=-L$$PWD/Python/libs/ -lpython311
    SOURCES += IdentityThread.cpp
    HEADERS += IdentityThread.h
}

# 使用C++
contains(DEFINES,UseC++){
    SOURCES += IdentityThreadC++.cpp
    HEADERS += IdentityThreadC++.h
    INCLUDEPATH += F:/MyLib/PyTorch/Release/include
    INCLUDEPATH += F:/MyLib/opencv/build/include

    win32:CONFIG(release, debug|release): LIBS += -LF:/MyLib/PyTorch/Release/lib/ -ltorch -lasmjit -lc10 -lc10_cuda -lcaffe2_nvrtc -lclog -lcpuinfo -ldnnl -lfbgemm -lfbjni -lfmt -lkineto\
    -llibprotobuf-lite\
    -llibprotobuf\
    -llibprotoc\
    -lnvfuser_codegen\
    -lpthreadpool\
    -lpytorch_jni\
    -ltorch_cpu\
    -ltorch_cuda\
    -lXNNPACK
    else:win32:CONFIG(debug, debug|release): LIBS += -LF:/MyLib/PyTorch/Debug/lib/ -ltorch -lasmjit -lc10 -lc10_cuda -lcaffe2_nvrtc -lclog -lcpuinfo -ldnnl -lfbgemm -lfbjni -lfmtd -lkineto\
    -llibprotobuf-lited\
    -llibprotobufd\
    -llibprotocd\
    -lnvfuser_codegen\
    -lpthreadpool\
    -lpytorch_jni\
    -ltorch_cpu\
    -ltorch_cuda\
    -lXNNPACK

    INCLUDEPATH += F:/MyLib/PyTorch/Release/lib
    DEPENDPATH += F:/MyLib/PyTorch/Release/lib

    win32:CONFIG(release, debug|release): LIBS += -LF:/MyLib/opencv/build/x64/vc16/lib/ -lopencv_world480
    else:win32:CONFIG(debug, debug|release): LIBS += -LF:/MyLib/opencv/build/x64/vc16/lib/ -lopencv_world480d

    INCLUDEPATH += F:/MyLib/opencv/build/x64/vc16/lib
    DEPENDPATH += F:/MyLib/opencv/build/x64/vc16/lib

}




