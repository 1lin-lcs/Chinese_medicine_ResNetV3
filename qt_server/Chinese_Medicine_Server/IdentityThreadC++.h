#ifndef IDENTITYTHREADC_H
#define IDENTITYTHREADC_H
#include <QThread>
#undef slots
#include <torch/script.h>
#include <torch/csrc/api/include/torch/cuda.h>
#include <ATen/core/TensorBody.h>
#define slots Q_SLOTS
#include <opencv2/opencv.hpp>
#include <QJsonObject>

using torch::jit::script::Module;

class IdentityThreadCpp : public QThread{
    Q_OBJECT
public:
    IdentityThreadCpp(QString,QJsonObject*,qintptr,Module);
    ~IdentityThreadCpp();
private:
    QString m_filename;
    QJsonObject* m_categories;
    qintptr m_socketDesc;
    Module m_Module;

protected:
    void run();

signals:
    void SendResult(QStringList,qintptr);
};

#endif // IDENTITYTHREADC_H
