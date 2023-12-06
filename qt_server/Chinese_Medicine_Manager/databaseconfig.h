#ifndef DATABASECONFIG_H
#define DATABASECONFIG_H

#include <QDialog>
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QMetaObject>

/*!
 * \brief The DatabaseInfo struct
 * \note 不要使用memset这类函数初始化
 */
struct DatabaseInfo{
    QString hostname;
    int port;
    QString databaseName;
    QString username;
    QString password;
};
Q_DECLARE_METATYPE(DatabaseInfo)

namespace Ui {
class DataBaseConfig;
}

class DataBaseConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DataBaseConfig(QWidget *parent = nullptr);
    ~DataBaseConfig();
    void setInfo(DatabaseInfo*);

private:
    void myConnect();

private slots:
    void showPasswd_toggled(bool);
    void buttonGroup_accepted();
    bool writeConfig();

private:
    Ui::DataBaseConfig *ui;
    QRegularExpression reg;
    QRegularExpressionValidator* regvalidor;

signals:
    void sendConfig(DatabaseInfo);
};

#endif // DATABASECONFIG_H
