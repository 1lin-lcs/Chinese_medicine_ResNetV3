#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QList>
#include <QSqlQuery>
#include "databaseconfig.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void myConnect();
    bool readConfig();
    bool connectDB();
    bool createModel();
    QStringList getHeaderNames();
    void init();
    void updateButtonState();

private slots:
    void QuitAction_triggered();
    void DBAction_triggered();
    void ComcoBox_itemChanged(QString);
    void ReLoadButton_clicked();
    void dialog_getConfig(DatabaseInfo);
    void AddButton_clicked();
    void DeleteButton_clicked();
    void RevertButton_clicked();
    void SubmitButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel* m_sqlTableModel=nullptr;
    QSqlDatabase m_db;
    DatabaseInfo* m_databaseInfo=new DatabaseInfo();
    QString m_currentTable="";
    bool m_isConnected=false;
};
#endif // MAINWINDOW_H
