#ifndef FINDDATA_H
#define FINDDATA_H

#include <QDialog>
#include <QPair>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlError>

namespace Ui {
class FindData;
}

class FindData : public QDialog
{
    Q_OBJECT

public:
    explicit FindData(QWidget *parent = nullptr);
    FindData(QWidget *parent,QList<QPair<QString,QString>> headers,QString currentTable,QSqlDatabase db);
    ~FindData();

private:
    bool LoadHeaderComboBox();
    void myConnect();

private slots:
    void FindButton_clicked();

private:
    Ui::FindData *ui;
    QList<QPair<QString,QString>> m_headerLists;
    QString m_currentTable;
    QSqlDatabase m_db;
};

#endif // FINDDATA_H
